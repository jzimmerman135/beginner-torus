CC := gcc
CFLAGS := -Wall -Werror -Wextra -Ofast
LDFLAGS :=
LDLIBS := -lncurses -lm
OBJECTS := vec.o

target: main

%.o: %.c
	@echo "compiling $< as $@"
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

main: $(OBJECTS) main.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf *.o main debug.txt

debug: 
	@./main 2> debug.txt