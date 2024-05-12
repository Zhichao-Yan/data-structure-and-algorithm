CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra  -pedantic -std=c99 -Iinclude
CXXFLAGS := -Wall -Wextra -pedantic -std=c++11 -Iinclude
CPPFLAGS :=
LDFLAGS := 
BUILD := bin
SRC := src
TEST := test
INCLUDE:= include

# 源文件
SOURCES := $(shell find $(SRC) -name '*.cpp')
# 源文件对于目标文件
OBJECTS := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SOURCES))

# 测试文件
TEST_SRC := $(shell find $(TEST) -name '*.cpp')
# 测试文件可执行文件
TEST_EXE := $(patsubst $(TEST)/%.cpp,%,$(TEST_SRC))

all: $(TEST_EXE)

%: $(TEST)/%.cpp $(SOURCES)
	@mkdir  -p $(BUILD)
	$(CC) $(CFLAGS) $^ -o $(BUILD)/$@

clean:
	rm -rf $(BUILD)

.PHONY: all clean

