
CC = gcc
CFLAGS =

OBJECTS = \
	gpio.o \
	i2c.o \
	ssd1311.o \
	main.o \
	scr.o

TARGET = ssd1311

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

gpio.o : src/gpio.c
	$(CC) $(CFLAGS) -c src/gpio.c

i2c.o : src/i2c.c
	$(CC) $(CFLAGS) -c src/i2c.c

ssd1311.o : src/ssd1311.c src/ssd1311.h
	$(CC) $(CFLAGS) -c src/ssd1311.c

scr.o : src/scr.c src/ssd1311.h
	$(CC) -c src/scr.c

main.o: src/main.c
	$(CC) -c src/main.c

clean:
	rm -rfv *.o

distclean:
	rm -rfv *.o ssd1311


