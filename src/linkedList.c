#include "linkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void insertNode(node_s **head, char id[], char lat[], char log[])
{
    node_s *newNode = (node_s *)malloc(sizeof(node_s));
    strcpy(newNode->id, id);
    strcpy(newNode->lat, lat);
    strcpy(newNode->log, log);


    newNode->next = (*head)->next;
    (*head)->next = newNode;
}

void initList(node_s **head)
{
    *head = (node_s *)malloc(sizeof(node_s));
    (*head)->next = NULL;
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
        printf("ID - %s Lat[%s] Log[%s]\n", current->id, current->lat, current->log);
        current = current->next;
    }
}