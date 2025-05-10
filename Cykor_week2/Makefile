CC = gcc
CFLAGS = -Wall -g
TARGET = main
OBJS = main.o gen_func.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c gen_func.h color.h
	$(CC) $(CFLAGS) -c main.c

gen_func_.o: gen_func.c gen_func.h
	$(CC) $(CFLAGS) -c gen_func.c

clean:
	rm -f $(OBJS) $(TARGET)