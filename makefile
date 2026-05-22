CC := g++
LDFLAGS := -lm

TARGET := minirede

SRC := minirede.cpp
OBJ := $(SRC:.cpp=.o)

INPUT ?= testes/pequeno/entrada.txt

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET)
	clear

run: $(TARGET)
	clear
	./$(TARGET)

test: $(TARGET)
	clear
	./$(TARGET) < $(INPUT)