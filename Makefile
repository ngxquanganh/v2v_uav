CC = gcc
CFLAGS += -Wall -g -Wno-unused-variable -pthread -Wno-unused-function

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

OBJ = $(OBJ_DIR)/main.o  $(OBJ_DIR)/cJSON.o

EXE = main

all: $(EXE)

$(EXE): $(OBJ)
	$(CC)  $(OBJ) -o $(EXE) $(CFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/cJSON.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/cJSON.o: $(SRC_DIR)/cJSON.c $(INC_DIR)/cJSON.h| $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/cJSON.c -o $(OBJ_DIR)/cJSON.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
clean:
	rm -f $(EXE) $(OBJ)