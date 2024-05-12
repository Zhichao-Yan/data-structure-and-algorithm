# 设置编译器和编译选项
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=c99 -Isrc
LDFLAGS :=
BIN_DIR := bin
SRC_DIR := src
TEST_DIR := test

# 自动发现源文件和目标文件
SOURCES := $(shell find $(SRC_DIR) -name "*.c")
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SOURCES))
EXECUTABLE := $(BIN_DIR)/main

# 测试相关
TEST_SOURCES := $(shell find $(TEST_DIR) -name "*.c")
TEST_EXECUTABLES := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/test_%,$(TEST_SOURCES))

# 目标规则
all: $(EXECUTABLE) $(TEST_EXECUTABLES)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BIN_DIR)/test_%: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $(@:.exe=.o)
	$(CC) $(LDFLAGS) $@.o -o $@
	@rm -f $@.o # 清理测试用例的.o文件

run-tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do $$test; done

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run-tests clean