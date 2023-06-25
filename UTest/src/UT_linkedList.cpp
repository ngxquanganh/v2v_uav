#include <gtest/gtest.h>
#include "linkedList.h"
#include "util.h"

volatile unsigned int num_uav;

class UT_linkedList : public testing::Test
{
protected:
    node_s *head;
    char *buffer;

    void SetUp() override
    {

        head = NULL;
        initList(&head);
        node_status_e ret = NODE_ERROR;

        buffer = (char *)calloc(1, PAYLOAD_MAX_SIZE);
        strcpy(buffer, "arrays support various string manipulation operations");
        ret = insertNode(&head, "01", buffer, 0,(char*) "0");

        insertNode(&head, "02", buffer, 1,(char*) "0");
        insertNode(&head, "03", buffer, 2,(char*) "0");
    }

    void TearDown() override
    {
    }
};

TEST_F(UT_linkedList, insertNode)
{

    EXPECT_STREQ("03", head->next->id);

    // PRINT_DEBUG("num node%d\n", num_uav);
}

TEST_F(UT_linkedList, updateNode)
{

    printList(head);
    updateNode(head, "02", 14, buffer,(char*)"0");

    printList(head);
    // PRINT_DEBUG("num node%d\n", num_uav);
}

TEST_F(UT_linkedList, deleteNode)
{

    printList(head);
    deleteNode(&head, "02");

    printList(head);
}

TEST_F(UT_linkedList, searchNode)
{

    node_s *node = searchNode(head, "02");
    EXPECT_EQ(node->seq_nb, 1);
}

TEST_F(UT_linkedList, travelList)
{
    node_s *node;

    while (node = travelList(head), node)
    {
        PRINT_DEBUG("%s\n", node->id);
    }
}