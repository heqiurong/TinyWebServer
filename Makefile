# 更新后的Makefile
CC = g++
CFLAGS = -std=c++11 -Wall
SRC = src/small_ptr.cpp
TARGET = bin/small_ptr
# TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
    $(CC) $(CFLAGS) $^ - o $@

clean:
    rm -f $(TARGET)

.PHONY: all clean