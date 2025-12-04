CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
SRCS = src/main.cpp src/instr.cpp src/symtab.cpp src/tokenizer.cpp src/util.cpp
TARGET = asm

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean