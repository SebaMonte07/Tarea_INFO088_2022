CC=g++
CFLAGS=-Wall -std=c++17
BINS=tarea
all: clean TAREA 

TAREA:
	$(CC) $(CFLAGS) -o tarea Tarea.cpp

clean:
	@echo " [CLN] Removing binary files"
	@rm -f $(BINS)
