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

int main()
{
    node_s *head;
    node_s *_node = (node_s *)calloc(1, sizeof(node_s));
    head = NULL;
    initList(&head);
    char *buffer = (char *)calloc(1, PAYLOAD_MAX_SIZE);
    strcpy(buffer, "arrays support various string manipulation operations");
 
    node_status_e ret = NODE_ERROR; 
    ret = insertNode(&head, "01", 0, buffer);

    insertNode(&head, "02", 1, buffer);
    insertNode(&head, "03", 2, buffer);
    insertNode(&head, "04", 3, buffer); 

    strcpy(_node->id, "02");
    strcpy(_node->buffer, "Hello world");
    _node->seq_nb = 15;


    printList(head);
}