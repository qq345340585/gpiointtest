.SUFFIXES : .x .o .c .s

CC := arm-linux-gcc
STRIP := arm-linux-strip

TARGET = gpiointtest_epoll.so
SRCS := gpiointtest_epoll.c gpioconfig.c
CFGFLAG := -fPIC -shared -L ./

all: 
	$(CC) $(CFGFLAG) $(SRCS) -o $(TARGET)
	$(STRIP) $(TARGET) 

clean:
	rm -f *.o 
	rm -f *.x 
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)	
	rm -f *.gdb
	rm -f *.bak
