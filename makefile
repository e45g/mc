SRCS = $(wildcard src/*.c)
FLAGS = -Wall -Wextra -lm
TARGET = mc

all:
	gcc $(SRCS) $(FLAGS) -o $(TARGET)
	./$(TARGET)
