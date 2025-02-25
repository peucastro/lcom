CC = clang
CFLAGS = -Wall -Wextra
LDFLAGS = -lm
TARGET = ch5
SRC = vector.c ch5.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
