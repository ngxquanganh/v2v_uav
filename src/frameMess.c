/**
 * @file frameMess.c
 * @brief Frame message
 * @date 2021-8-1
 * @version v1.0.0
 * @note Last update on 2021-8-1
 * @note
 * - v1.0.0(2021-8-1):
 *  -# Frame message
 */

#include "frameMess.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Message_status_t serializeMessage(Message_data_t *message)
{
   unsigned int bufItr = 0;
   memcpy(message->buffer, &message->type, sizeof(message->type));
   bufItr += sizeof(message->type);
   memcpy(message->buffer + bufItr, &message->id, sizeof(message->id));
   bufItr += sizeof(message->id);
   memcpy(message->buffer + bufItr, &message->seq_nb, sizeof(message->seq_nb));
   bufItr += sizeof(message->seq_nb);
   memcpy(message->buffer + bufItr, &message->payload, sizeof(message->payload));
   bufItr += sizeof(message->payload);

   return MSG_OK;
}

Message_status_t deserializeMessage(Message_data_t *message)
{
   unsigned int bufItr = 0;
   memcpy(&message->type, message->buffer, sizeof(message->type));
   bufItr += sizeof(message->type);
   memcpy(&message->id, message->buffer + bufItr, sizeof(message->id));
   bufItr += sizeof(message->id);
   memcpy(&message->seq_nb, message->buffer + bufItr, sizeof(message->seq_nb));
   bufItr += sizeof(message->seq_nb);
   memcpy(&message->payload, message->buffer + bufItr, sizeof(message->payload));
   bufItr += sizeof(message->payload);

   
   return MSG_OK;
}