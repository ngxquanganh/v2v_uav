#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>

#include "../include/frameMess.h"
#include "../include/cJSON.h"
#include "../include/linkedList.h"
#include "../include/util.h"

#define BUF_SIZE 1024
#define TIME_RECV 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
node_s *head = NULL;

static char my_uav_id[ID_SIZE];
static unsigned int sequence_pkt_nb = 0;

static void *recv_thread(void *arg)
{

    int sock;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len = sizeof(remote_addr);
    char buffer_recv[BUFFER_MAX_SIZE];
    time_t timestamp;

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    char ip[5][16];
    uint8_t nb_interface;

    nb_interface = 0;

    Message_data_t *mess;

    node_s *node;

    mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));

    // TODO: Assert alloc and init

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    // Iterate over the list of interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // Check if it's an IPv4 address
        if (family == AF_INET)
        {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            strcpy(ip[nb_interface++], host);
        }
    }

    printf("nb interface %d'\n", nb_interface);
    freeifaddrs(ifaddr);
    // Create a UDP socket and bind it to the local address
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set timeout for receive
    struct timeval tv;
    tv.tv_sec = TIME_RECV;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("Error");
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all local interfaces
    local_addr.sin_port = htons(*(uint16_t *)arg);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening for broadcast messages on port %d...\n", *(uint16_t *)arg);

    for (;;)
    {

        timestamp = time(NULL);

        pthread_mutex_lock(&mutex);
        // Receive the message from the server
        // Wait for enough receive time
        while ((time(NULL) - timestamp) < TIME_RECV)
        {

            if (recvfrom(sock, buffer_recv, BUFFER_MAX_SIZE, 0, (struct sockaddr *)&remote_addr, &addr_len) < 0)
            {

                if (errno == EAGAIN)
                {
                    printf("Timeout\n");
                    goto END_LOOP;
                }
                perror("receive fail:");
            }

            // Convert ip from sender to string
            char str[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &(remote_addr.sin_addr), str, INET_ADDRSTRLEN) == NULL)
            {
                perror("inet_ntop");
                continue;
            }

            uint8_t send_myself_flag = 0;

            // Check if sender ip is myself
            for (int i = 0; i < nb_interface; i++)
            {
                if (!strcmp(str, ip[i]))
                {

                    send_myself_flag = 1;
                    continue;
                }
            }

            if (send_myself_flag)
                continue;

            unsigned int ip = ntohl(remote_addr.sin_addr.s_addr);
            unsigned int d1 = (ip >> 24) & 0xFF;
            unsigned int d2 = (ip >> 16) & 0xFF;
            unsigned int d3 = (ip >> 8) & 0xFF;
            unsigned int d4 = ip & 0xFF;

            memcpy(mess->buffer, buffer_recv, BUFFER_MAX_SIZE);
            Message_status_t ret_mess = deserializeMessage(mess);

            if (ret_mess)
            {
                printf("error mess\n");
                // TODO: log error
            }

            printf(">> Message: uav id [%s] addr [%03u.%03u.%03u.%03u] seq_nb [%d] [%s]\n", mess->id, d1, d2, d3, d4, mess->seq_nb, mess->payload);
            node = searchNode(head, mess->id);

            if (!node)
            {
                
                if (memcmp(mess->id, my_uav_id, 2))
                {
                    insertNode(&head, mess->id, mess->payload, mess->seq_nb);
                    PRINT_DEBUG("> insert node\n");
                }
            }
            else
            {
                if (memcmp(mess->id, my_uav_id, 2))
                {
                    if (mess->seq_nb > node->seq_nb)
                    {
                        updateNode(head, mess->id, mess->seq_nb, mess->payload);
                        PRINT_DEBUG("> updateNode\n");
                    }
                    else
                    {
                        PRINT_DEBUG("> old packet\n");
                    }
                }
                else
                {
                    PRINT_DEBUG("> packet of me\n");
                }
            }

            printList(head);
        }

    END_LOOP:
        printList(head);
        pthread_mutex_unlock(&mutex);
        printf("Receive end\n----------------\n\n");
        usleep(10);
    }
    // Close the socket
    close(sock);

    pthread_exit(NULL);
}

static void *send_thread(void *arg)
{
    int sock;
    struct sockaddr_in broadcast_addr;
    char message[BUF_SIZE];

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    node_s *node;

    Message_data_t *mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));

    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow broadcasting
    int broadcast_enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set broadcast address
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(*(uint16_t *)arg);

    for (;;)
    {
        pthread_mutex_lock(&mutex);
        if (getifaddrs(&ifaddr) == -1)
        {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }

        // Iterate over the list of interfaces
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL)
                continue;
            if (!(strcmp(ifa->ifa_name, "lo")))
                continue;

            family = ifa->ifa_addr->sa_family;

            // Check if it's an IPv4 address
            if (family == AF_INET)
            {
                struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
                struct sockaddr_in *broadcast = (struct sockaddr_in *)ifa->ifa_broadaddr;

                s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                if (s != 0)
                {
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }

                char broadcastAddress[NI_MAXHOST];
                if (broadcast != NULL)
                {
                    s = getnameinfo((struct sockaddr *)broadcast, sizeof(struct sockaddr_in), broadcastAddress, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if (s != 0)
                    {
                        printf("getnameinfo() failed: %s\n", gai_strerror(s));
                        exit(EXIT_FAILURE);
                    }
                }

                // printf("Interface: %s\n", ifa->ifa_name);
                // printf("IP Address: %s\n", host);
                // printf("Broadcast Address: %s\n", (broadcast != NULL) ? broadcastAddress : "N/A");

                broadcast_addr.sin_addr.s_addr = inet_addr(broadcastAddress); // Broadcast address for local network

                // Send the message to all clients on the local network
                while (node = travelList(head), node)
                {
                    memset(mess->buffer, 0, sizeof(mess->buffer));
                    memset(mess->payload, 0, sizeof(mess->payload));
                    mess->seq_nb = node->seq_nb;
                    mess->type = MSG_TYPE_DATA;
                    memcpy(mess->id, node->id, ID_SIZE);
                    memcpy(mess->payload, node->buffer, PAYLOAD_MAX_SIZE);
                    serializeMessage(mess);

                    if (sendto(sock, mess->buffer, sizeof(mess->buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0)
                    {
                        perror("sendto failed");
                        // exit(EXIT_FAILURE);
                    }
                    else
                    {
                        printf("Message [%s][%d] sent to all clients on the network %s\n", mess->id, mess->seq_nb, broadcastAddress);
                    }
                }

                /* send my info */
                memset(mess->buffer, 0, sizeof(mess->buffer));
                memset(mess->payload, 0, sizeof(mess->payload));
                mess->seq_nb = sequence_pkt_nb;
                mess->type = MSG_TYPE_DATA;
                memcpy(mess->id, my_uav_id, ID_SIZE);
                memcpy(mess->payload, "this is dump payload", 20);
                serializeMessage(mess);

                if (sendto(sock, mess->buffer, sizeof(mess->buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0)
                {
                    perror("sendto failed");
                    // exit(EXIT_FAILURE);
                }
                else
                {
                    sequence_pkt_nb++;
                    printf("Message [%s][%d] sent to all clients on the network %s\n", mess->id, mess->seq_nb, broadcastAddress);
                }
            }
        }

        freeifaddrs(ifaddr);

        printf("Send end\n----------------\n\n");

        pthread_mutex_unlock(&mutex);

        // For receive thread acquire mutex
        usleep(100);
    }
    // Close the socket
    close(sock);

    pthread_exit(NULL);
}

int main()
{
    char source[1000];
    pthread_t thread_id;
    int ret;

    FILE *fp = fopen("config.json", "r");

    if (fp != NULL)
    {
        int count = fread(&source, sizeof(char), 100, fp);
        fclose(fp);
    }
    else
    {
        printf("open fail");
    }

    // Parse JSON
    cJSON *config = cJSON_Parse(source);

    // Get id and listen port
    cJSON *id_c = cJSON_GetObjectItem(config, "id");
    strcpy(my_uav_id, id_c->valuestring);
    uint16_t listen_port = cJSON_GetObjectItem(config, "rec_port")->valueint;

    initList(&head);
    // assert this init

    ret = pthread_create(&thread_id, NULL, recv_thread, (void *)&listen_port);
    if (ret)
    {
        perror("Pthread create fail: ");
    }
    usleep(100);
    ret = pthread_create(&thread_id, NULL, send_thread, (void *)&listen_port);
    if (ret)
    {
        perror("Pthread create fail: ");
    }
    for (;;)
        pause();
    return 0;
}
