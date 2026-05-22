CC := g++
LDFLAGS := -lm

TARGET := minirede.out

SRC := minirede.cpp
OBJ := $(SRC:.cpp=.o)

.PHONY: all clean run

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