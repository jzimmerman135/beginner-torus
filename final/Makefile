CC := gcc
CFLAGS := -Wall -Werror -Wextra -Ofast
LDFLAGS :=
LDLIBS := -lncurses

OBJECTS := vec.o

target: main mini tiny

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

main: main.o vec.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

mini: mini.o
	echo "$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)"
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

clean:
	rm -rf *.o main mini
