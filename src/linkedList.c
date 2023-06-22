#include "linkedList.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
node_s *nodeIter;

node_status_e insertNode2(node_s **head, char *id, char *buffer, unsigned int seq_nb)
{

    node_s *newNode = (node_s *)calloc(1, sizeof(node_s));
    if (newNode == NULL)
    {
        PRINT_DEBUG("Error allocating memory");
        return NODE_ERROR;
    }
    memcpy(newNode->id, id, ID_SIZE);
    newNode->seq_nb = seq_nb;
    memcpy(newNode->buffer, buffer, PAYLOAD_MAX_SIZE);
    newNode->next = (*head)->next;
    (*head)->next = newNode;
    num_uav++;
    return NODE_OK;
}

void initList(node_s **head)
{
    PRINT_DEBUG("Init list\n");
    *head = (node_s *)malloc(sizeof(node_s));
    (*head)->next = NULL;
    nodeIter = *head;
}

void deleteNode(node_s **head, char id[])
{
    node_s *current = *head;
    node_s *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->id, id) == 0)
        {
            if (previous == NULL)
            {
                *head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            num_uav--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

void printList(node_s *head)
{
    node_s *current = head->next;
    while (current != NULL)
    {
        printf("ID [%s] seq_nb [%d] \n", current->id, current->seq_nb);
        current = current->next;
    }
}

node_s *searchNode(node_s *head, char *id)
{

    node_s *current;
    // node_s *return_node = (node_s *)malloc(sizeof(node_s));
    current = head->next;

    if (current == NULL)
    {
        PRINT_DEBUG("NULL");
    }
    while (current != NULL)
    {
        PRINT_DEBUG("Current id [%s] \n", current->id);
        if (memcmp(current->id, id, 2) == 0)
        {
            printf("Found node with id [%s] \n", current->id);
            // memcpy(return_node, current, sizeof(node_s));
            // return return_node;
            return current;
        }
        current = current->next;
    }
    return NULL;
}

node_status_e updateNode(node_s *head, char *id, unsigned int seq_nb, char *buffer)
{
    node_s *current = head->next;

    while (current != NULL)
    {
        if (!memcmp(current->id, id, 2))
        {
            current->seq_nb = seq_nb;
            memcpy(current->buffer, buffer, PAYLOAD_MAX_SIZE);
            return NODE_OK;
        }
        current = current->next;
    }
    return NODE_ERROR;
}


node_s *travelList(node_s *head)
{
    if (nodeIter->next == NULL)
    {
        nodeIter = head;
        return NULL;
    }
    else
    {
        nodeIter = nodeIter->next;
        return nodeIter;
    }
}