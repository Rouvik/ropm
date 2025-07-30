.PHONY: all prepare clean install

CC := gcc

SRC_DIR = ./src
BUILD_DIR := ./bin
INCLUDE_DIR = ./include

TARGETS = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(TARGETS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# DEBUG := -ggdb
CFLAGS = -Wall -I$(INCLUDE_DIR) -lcrypto $(DEBUG)
OBJFLAGS = $(CFLAGS)

all: prepare $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o $(BUILD_DIR)/main

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(OBJFLAGS) $< -c -o $@

prepare:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)