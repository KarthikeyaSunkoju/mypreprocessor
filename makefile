CC = gcc
CFLAGS = -Wall -Wextra

TARGET = mypreprocessor

SRCS = main.c \
       preprocessor.c \
       comment_removal.c \
       file_inclusion.c \
       macro_handler.c

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
