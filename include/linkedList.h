#pragma once

typedef struct node
{
    char id[6];

    char lat[10];

    char log[10];

    struct node *next;

} node_s;

/**
 * @brief Insert a node at the end of the list
 * @param head The head of the list
 * @param id The id of the node
 * @param lat The latitude of the node
 * @param log The longitude of the node
 * @return void
 */
void insertNode(node_s **head, char id[], char lat[], char log[]);

/**
 * @brief Print the list
 * @param head The head of the list
 * @return void
 */
void initList(node_s **head);

/**
 * @brief Delete a node from the list
 * @param head The head of the list
 * @param id The id of the node
 * @return void
 */
void deleteNode(node_s **head, char id[]);

/**
 * @brief Print the list
 * @param head The head of the list
 * @return void
 * @note This function is used for debugging
 */
void printList(node_s *head);
