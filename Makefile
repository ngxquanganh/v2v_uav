CC = gcc
CFLAGS += -Wall -g3 -Wno-unused-variable -pthread -Wno-unused-function 
# CFLAGS +=   -fprofile-arcs -ftest-coverage
# CFLAGS += -fsanitize=address -static-libasan

SRC_DIR = src
SRC_TEST_DIR = test/src
OBJ_DIR = obj


OBJ_TEST_DIR = test/obj
INC_DIR = include

GCNO = $(OBJ_DIR)/main.gcno  $(OBJ_DIR)/cJSON.gcno  $(OBJ_DIR)/linkedList.gcno $(OBJ_DIR)/frameMess.gcno
GCDA = $(OBJ_DIR)/main.gcda  $(OBJ_DIR)/cJSON.gcda  $(OBJ_DIR)/linkedList.gcda $(OBJ_DIR)/frameMess.gcda

OBJ = $(OBJ_DIR)/main.o  $(OBJ_DIR)/cJSON.o  $(OBJ_DIR)/linkedList.o $(OBJ_DIR)/frameMess.o 
OBJ_TEST = $(OBJ_TEST_DIR)/main.o  $(OBJ_TEST_DIR)/cJSON.o  $(OBJ_TEST_DIR)/linkedList.o $(OBJ_TEST_DIR)/frameMess.o

EXE = main
TEST = main_test

all: $(EXE)

$(EXE): $(OBJ)
	$(CC)  $(OBJ) -o $(EXE) $(CFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main1.c $(INC_DIR)/cJSON.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/main1.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/linkedList.o: $(SRC_DIR)/linkedList.c $(INC_DIR)/linkedList.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/linkedList.c -o $(OBJ_DIR)/linkedList.o

$(OBJ_DIR)/frameMess.o: $(SRC_DIR)/frameMess.c $(INC_DIR)/frameMess.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/frameMess.c -o $(OBJ_DIR)/frameMess.o

$(OBJ_DIR)/cJSON.o: $(SRC_DIR)/cJSON.c $(INC_DIR)/cJSON.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/cJSON.c -o $(OBJ_DIR)/cJSON.o

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# test: $(TEST)

# $(TEST): $(OBJ_TEST)
# 	$(CC)  $(OBJ_TEST) -o $(TEST) $(CFLAGS)

# $(OBJ_TEST_DIR)/main.o: $(SRC_TEST_DIR)/main.c $(INC_DIR)/cJSON.h| $(OBJ_TEST_DIR)
# 	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_TEST_DIR)/main.c -o $(OBJ_TEST_DIR)/main.o

# $(OBJ_TEST_DIR)/linkedList.o: $(SRC_DIR)/linkedList.c $(INC_DIR)/linkedList.h| $(OBJ_TEST_DIR)
# 	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/linkedList.c -o $(OBJ_TEST_DIR)/linkedList.o

# $(OBJ_TEST_DIR)/frameMess.o: $(SRC_DIR)/frameMess.c $(INC_DIR)/frameMess.h| $(OBJ_TEST_DIR)
# 	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/frameMess.c -o $(OBJ_TEST_DIR)/frameMess.o

# $(OBJ_TEST_DIR)/cJSON.o: $(SRC_DIR)/cJSON.c $(INC_DIR)/cJSON.h| $(OBJ_TEST_DIR)
# 	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/cJSON.c -o $(OBJ_TEST_DIR)/cJSON.o

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_TEST_DIR)

clean:
	rm -f $(EXE) $(OBJ) $(GCNO) $(GCDA) 
	rm -f $(TEST) $(OBJ_TEST)