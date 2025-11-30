PROG = main.exe
SRC = *.c
CFLAGS = -Wall -Werror -g -std=c99
LIBS = 
all: $(PROG)
$(PROG): $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(LIBS)
clean:
	rm -f $(PROG)
.PHONY: all clean