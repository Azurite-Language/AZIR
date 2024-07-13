SRC=src/
OBJ_DIR=obj
CPP_FILES=$(wildcard $(SRC)*.cpp)
OBJ_FILES=$(patsubst $(SRC)%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
DEPS=$(patsubst $(SRC)%.cpp,%.d,$(CPP_FILES))
CC=g++
OBJ=obj/
TARGET=bin/AZIR
CXXFLAGS=-std=c++17 -Wall -Wextra -Wpedantic -Werror -g -Iinclude -MMD -Wno-unused-variable -Wno-unused-parameter -O0

all: folder $(TARGET)

folder:
	mkdir -p $(OBJ_DIR)
	mkdir -p bin

.PHONY: all clean folder

$(OBJ)%.o: $(SRC)%.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

-include $(DEPS)