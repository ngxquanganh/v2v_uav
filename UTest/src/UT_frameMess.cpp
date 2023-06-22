#include <gtest/gtest.h>
#include "frameMess.h"
#include "util.h"
#include <string.h>

class UT_frameMess : public testing::Test
{
protected:
    Message_data_t *mess;

    void SetUp() override
    {
        mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));
    }

    void TearDown() override
    {
        free(mess);
    }
};

class UT_frameMess_MessType : public testing::TestWithParam<Message_type_t>
{
protected:
    Message_data_t *mess;

    void SetUp() override
    {
        mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));
    }

    void TearDown() override
    {
        free(mess);
    }
};
class UT_frameMessParamSeqNb : public testing::TestWithParam<int>
{
protected:
    Message_data_t *mess;

    void SetUp() override
    {
        mess = (Message_data_t *)calloc(1, sizeof(Message_data_t));
    }

    void TearDown() override
    {
        free(mess);
    }
};

TEST_F(UT_frameMess, testSerialize)
{
    memcpy(mess->id, "01", 2);
    mess->seq_nb = 1;
    mess->type = MSG_TYPE_DATA;
    memcpy(mess->payload, "123.123456", 10);

    serializeMessage(mess);

    printf("mess->buffer: %s\n", (mess->payload));
}

TEST_F(UT_frameMess, testSerialize1)
{
    GTEST_SKIP() << "skip";
    memcpy(mess->id, "01", 2);
    mess->seq_nb = 1;
    mess->type = MSG_TYPE_DATA;
    memcpy(mess->payload, "123.123456", 10);
    PRINT_DEBUG("size  mess->type %d", sizeof(mess->type));
    serializeMessage(mess);

    printf("mess->buffer: %s\n", (mess->payload));
}

TEST_F(UT_frameMess, testDeserialize)
{
    memcpy(mess->id, "01", 2);
    mess->seq_nb = 1;
    mess->type = MSG_TYPE_DATA;
    memcpy(mess->payload, "123.123456", 10);

    serializeMessage(mess);

    deserializeMessage(mess);

    EXPECT_STREQ("01", mess->id);
    EXPECT_EQ(1, mess->seq_nb);
    EXPECT_STREQ("123.123456", mess->payload);
    EXPECT_EQ(MSG_TYPE_DATA, mess->type);
}

/* TEST_P(UT_frameMess_MessType, test_mess_type)
{
    GTEST_SKIP() << "skippp";
    memcpy(mess->id, "01", 2);
    mess->seq_nb = 1;
    mess->type = GetParam();
    memcpy(mess->payload, "123.123456", 10);

    serializeMessage(mess);

    deserializeMessage(mess);

    EXPECT_STREQ("01", mess->id);
    EXPECT_EQ(1, mess->seq_nb);
    EXPECT_STREQ("123.123456", mess->payload);
    EXPECT_EQ(GetParam(), mess->type);
}

TEST_P(UT_frameMessParamSeqNb, test_sequence_number)
{
    GTEST_SKIP();
    memcpy(mess->id, "01", 2);
    mess->seq_nb = GetParam();
    mess->type = MSG_TYPE_DATA;
    memcpy(mess->payload, "123.123456", 10);

    serializeMessage(mess);

    deserializeMessage(mess);

    EXPECT_STREQ("01", mess->id);
    EXPECT_EQ(GetParam(), mess->seq_nb);
    EXPECT_STREQ("123.123456", mess->payload);
    EXPECT_EQ(MSG_TYPE_DATA, mess->type);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName1, UT_frameMess_MessType, ::testing::Values(MSG_TYPE_DATA, MSG_TYPE_ACK, MSG_TYPE_NACK));
INSTANTIATE_TEST_SUITE_P(InstantiationName2, UT_frameMessParamSeqNb, ::testing::Range(-1, 1)); */