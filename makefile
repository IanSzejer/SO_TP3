CC= gcc
GCCFLAGS= -std=gnu99  -Wall -pedantic 
GCCLIBS= -lm

SRC_SERVER= server.c
SRC_CLIENT= client.c
SRC_LEVELS= levelFunctions.c

OBJ_LEVELS=$(SRC_LEVELS:.c=.o)

EXEC_SERVER= server
EXEC_CLIENT= client

all: $(OBJ_LEVELS) client
	$(CC) $(GCCFLAGS) $(SRC_SERVER) $(OBJ_LEVELS) -I./include -o $(EXEC_SERVER) $(GCCLIBS)
	touch .RUN_ME
	objcopy --add-section .RUN_ME=.RUN_ME  server
	rm .RUN_ME

%.o: %.c
	$(CC) $(GCCFLAGS) -I./include -c $^ $(GCCLIBS)

client:
	$(CC) $(GCCFLAGS) $(SRC_CLIENT) -o $(EXEC_CLIENT)

clean:
	rm -rf *.o server client quine


.PHONY: all clean client server