CC := gcc
CFLAGS := -Wall -Werror -Wextra -Ofast
LDFLAGS :=
LDLIBS := -lncurses

OBJECTS := vec.o

target: main mini

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

main: main.o vec.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

mini: mini.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

clean:
	rm -rf *.o main mini
