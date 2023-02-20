MAKEFLAGS += --silent

TARGET_EXEC := huffman

CC = gcc
DEBUG_FLAGS = -g3
RELEASE_FLAGS = -O2
CFLAGS = 
CFLAGS += $(DEBUG_FLAGS)
#CFLAGS += $(RELEASE_FLAGS)
# CPPFLAGS = -D VERBOSE

BUILD_DIR := ./build
SRC_DIRS := ./src
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET_EXEC)
	rm -r $(BUILD_DIR)
	./$(TARGET_EXEC) test
.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
