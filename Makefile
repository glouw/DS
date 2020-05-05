OBJS = List

CC = gcc

CFLAGS = -Og -Wall -Wextra -Wpedantic -fsanitize=undefined -fsanitize=address

test: $(OBJS)
	@ for test in $(OBJS); do echo "TEST $$test"; ./$$test; done
	@ date >> test

List: List.[ch] Makefile
	@ echo CC $<
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
