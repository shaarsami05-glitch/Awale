CC     = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP
TARGET = awale
SRCS   = awale.c interface.c
OBJS   = $(SRCS:.c=.o)
DEPS   = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)

.PHONY: all clean
