#include <gtest/gtest.h>
#include "frameMess.h"
#include "linkedList.h"
#include "util.h"
#include <string.h>

class UT_handleMess : public testing::Test
{
protected:
    Message_data_t *mess;
    node_s *head;
    char *buffer;
    void SetUp() override
    {
        mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));
        head = NULL;
        initList(&head);

        // init a list
        buffer = (char *)calloc(1, PAYLOAD_MAX_SIZE);
        strcpy(buffer, "arrays support various string manipulation operations");
        insertNode(&head, "01", buffer, 0, (char *)"0");
        insertNode(&head, "02", buffer, 1, (char *)"0");
        insertNode(&head, "03", buffer, 2, (char *)"0");
    }

    void TearDown() override
    {
        free(mess);
    }
};

TEST_F(UT_handleMess, parse)
{
    Message_data_t *buff = (Message_data_t *)calloc(1, sizeof(Message_data_t));

    node_status_e ret = NODE_ERROR;

    // init a new message
    memcpy(buff->id, "01", 2);
    buff->seq_nb = 1;
    buff->type = MSG_TYPE_DATA;
    memcpy(buff->payload, "123.123456", 10);
    serializeMessage(buff);

    memcpy(mess->buffer, buff->buffer, BUFFER_MAX_SIZE);

    deserializeMessage(mess);

    node_s *node = searchNode(head, mess->id);
    EXPECT_EQ(node->seq_nb, 0);
    if (!node)
    {
        PRINT_DEBUG("not found!");
        /* insert */
    }
    else
    {
        updateNode(head, mess->id, mess->seq_nb, mess->payload, "0");
    }

    printList(head);
}
TEST_F(UT_handleMess, travel_serialize)
{

    node_s *node;
    int cnt = 0;

    while (node = travelList(head), node)
    {
        mess->seq_nb = node->seq_nb;
        memcpy(mess->id, node->id, ID_SIZE);
        memcpy(mess->payload, node->buffer, PAYLOAD_MAX_SIZE);
        serializeMessage(mess);

        PRINT_DEBUG("%d\n", cnt++);
    }

    cnt = 0;

    insertNode(&head, "06", buffer, 16 ,"0");
    while (node = travelList(head), node)
    {
        mess->seq_nb = node->seq_nb;
        memcpy(mess->id, node->id, ID_SIZE);
        memcpy(mess->payload, node->buffer, PAYLOAD_MAX_SIZE);
        serializeMessage(mess);

        PRINT_DEBUG("%d\n", cnt++);
    }
}
