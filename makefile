CFLAGS  = -std=c99 -g \
	-W -Wall -Wextra \

LFLAGS = -lm

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : mydht

hash: hash.o
	$(CC) $(CFLAGS) -o $@ $^

utils: utils.o
	$(CC) $(CFLAGS) -o $@ $^

mydht : mydht.o hash.o utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge :
	$(RM) mydht *.o
