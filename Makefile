CC := gcc
CSTD := -std=c17
CFLAGS := $(CSTD) -Wall -Wextra -Werror
CPPFLAGS := -D_GNU_SOURCE -I. -Isrc

OBJ_DIR := build/obj
BIN_DIR := build/bin
STATFORGE_TARGET := $(BIN_DIR)/statforge
TESTFORGE_TARGET := $(BIN_DIR)/testforge

COMMON_SRCS := \
	src/serializer/load_dump.c \
	src/serializer/store_dump.c

STATFORGE_SRCS := \
	src/stateforge/main.c \
	src/stateforge/data_processing/join_dump.c \
	src/stateforge/data_processing/sort_dump.c \
	$(COMMON_SRCS)

TESTFORGE_SRCS := \
	src/testforge/main.c \
	src/testforge/run_test_case.c \
	src/testforge/is_equal_statdata.c \
	$(COMMON_SRCS)

STATFORGE_OBJS := $(STATFORGE_SRCS:src/%.c=$(OBJ_DIR)/%.o)
TESTFORGE_OBJS := $(TESTFORGE_SRCS:src/%.c=$(OBJ_DIR)/%.o)

.PHONY: all build run clean rebuild

all: build

build: $(STATFORGE_TARGET) $(TESTFORGE_TARGET)

run: build
	$(TESTFORGE_TARGET) $(STATFORGE_TARGET)

rebuild: clean build

$(STATFORGE_TARGET): $(STATFORGE_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(STATFORGE_OBJS) -o $@

$(TESTFORGE_TARGET): $(TESTFORGE_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TESTFORGE_OBJS) -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) build/case_*.dump
