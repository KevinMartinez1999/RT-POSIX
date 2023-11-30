CC = gcc
CFLAGS = -Wall -g
LIBRT = -lrt
LIBPTHREAD = -lpthread

SETTINGS = periodic_settings
TASKS = task_definition
MAIN = main

all: $(MAIN)

$(SETTINGS).o: $(SETTINGS).c $(SETTINGS).h
	$(CC) $(CFLAGS) -c $(SETTINGS).c $(LIBRT) $(LIBPTHREAD)

$(TASKS).o: $(TASKS).c $(TASKS).h
	$(CC) $(CFLAGS) -c $(TASKS).c $(LIBRT) $(LIBPTHREAD)

$(MAIN): $(MAIN).c $(SETTINGS).o $(TASKS).o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT) $(LIBPTHREAD)

clean:
	rm -f *.o
	rm -f $(MAIN)
