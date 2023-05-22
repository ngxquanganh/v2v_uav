#include "linkedList.h"
#include <stdio.h>

node_s *head = NULL;


int main()
{
    initList(&head);
    insertNode(&head, "12345", "123.123", "123.123");
    insertNode(&head, "12346", "123.123", "123.123");
    insertNode(&head, "12347", "123.123", "123.123");

    printList(head);

    deleteNode(&head, "12346");

    printList(head);
    return 0;
}