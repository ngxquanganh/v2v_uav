#include <gtest/gtest.h>
#include "linkedList.h"
#include "util.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

class UT_write : public testing::Test
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
        ret = insertNode(&head, "01", buffer, 0, (char *)"0");

        insertNode(&head, "02", buffer, 1, (char *)"0");
        insertNode(&head, "03", buffer, 2, (char *)"0");
    }

    void TearDown() override
    {
    }
};

TEST_F(UT_write, test_write)
{
    int file_descriptor;
    struct flock lock;

    file_descriptor = open("/tmp/output.txt", O_WRONLY | O_APPEND | O_CREAT);

    if (file_descriptor == -1)
    {
        perror("Failed to open the file");
    }

    lock.l_type = F_WRLCK; // Shared lock for reading
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(file_descriptor, F_SETLKW, &lock) == -1)
    {
        perror("Failed to acquire file lock");
        close(file_descriptor);
    }

    char buffer[] = "helo mike fen\n";
    ssize_t bytesWritten = write(file_descriptor, buffer, sizeof(buffer) - 1);
    if (bytesWritten == -1)
    {
        perror("Failed to write to the file");
        close(file_descriptor);

    }

    printf("Data written to the file: %s\n", buffer);

    // Release the lock
    lock.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock) == -1)
    {
        perror("Failed to release file lock");
        close(file_descriptor);
    }

    // Close the file
    close(file_descriptor);
}