#ifdef __cplusplus
extern "C"
{
#endif
#pragma once
#include "frameMess.h"
    extern volatile unsigned int num_uav;

    typedef enum node_status
    {
        NODE_OK,
        NODE_ERROR
    } node_status_e;

    typedef struct node
    {
        char id[ID_SIZE];

        char buffer[PAYLOAD_MAX_SIZE];

        unsigned int seq_nb;

        struct node *next;

        char gcs_indicator[1];

    } node_s;

    /**
     * @brief Insert a node at the end of the list
     * @param head The head of the list
     * @param id The id of the node
     * @param seq_nb The sequence number of the node
     * @param buffer The buffer of the node
     * @return node_status_e The status of the insertion
     */
    node_status_e insertNode(node_s **head, char *id, char *buffer, unsigned int seq_nb, char *gcs_flag);

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

    /**
     * @brief Search a node in the list
     * @param head The head of the list
     * @param id The id of the node
     * @return node_s The node
     */
    node_s *searchNode(node_s *head, char id[]);

    /**
     * @brief Update a node in the list
     * @param head The head of the list
     * @param id The id of the node
     * @param seq_nb The sequence number of the node
     * @param buffer The buffer of the node
     * @return void
     */
    node_status_e updateNode(node_s *head, char id[], unsigned int seq_nb, char buffer[], char *gcs_flag);

    void printNode(node_s *head, node_s *_node);

    node_s *travelList(node_s *head);

#ifdef __cplusplus
}
#endif