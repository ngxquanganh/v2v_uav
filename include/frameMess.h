#ifdef __cplusplus
extern "C"
{
#endif

#pragma once

#include <stdint.h>
#include <time.h>

#define PAYLOAD_MAX_SIZE 100
#define BUFFER_MAX_SIZE 150
#define ID_SIZE 3


    typedef enum
    {
        MSG_OK = 0,
        MSG_ERROR = 1
    } Message_status_t;

    typedef enum
    {
        MSG_TYPE_DATA = 0,
        MSG_TYPE_ACK = 1,
        MSG_TYPE_NACK = 2,
        MSG_TYPE_END = 3,
        MSG_TYPE_ERROR = 4
    } Message_type_t;

    typedef struct
    {
        char buffer[BUFFER_MAX_SIZE];

        Message_type_t type;

        char payload[PAYLOAD_MAX_SIZE];

        char id[ID_SIZE];

        unsigned int seq_nb;

        time_t timestamp;

        char gcs_indicator[1];


    } Message_data_t;

    Message_status_t serializeMessage(Message_data_t *message);

    Message_status_t deserializeMessage(Message_data_t *message);

#ifdef __cplusplus
}
#endif
