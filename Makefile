BIN = test

CC = gcc

SRCS = main.c List.c Queue.c Stack.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

FLAGS = -O3 -march=native -flto -Wall -Wextra -Wshadow -Wpedantic -std=c11 -fsanitize=address -fsanitize=undefined

$(BIN): $(OBJS) $(DEPS)
	$(CC) $(FLAGS) $(OBJS) $(LIBS) -o $(BIN)
	@ echo ">> RUNNING TESTS"
	@ ./$(BIN)

%.o : %.c %.d Makefile
	$(CC) $(FLAGS) -MMD -MP -MT $@ -MF $*.d -c $<

-include *.d

%.d : ;

.PHONY clean:
clean:
	rm -f $(BIN)
	rm -f $(OBJS)
	rm -f $(DEPS)
