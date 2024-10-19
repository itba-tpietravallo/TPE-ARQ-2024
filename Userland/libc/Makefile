include ../Makefile.inc

MODULE=libc.o
SOURCES=$(wildcard [^_]*.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) -c -I../include $(GCCFLAGS) $(SOURCES) -o ../$(MODULE)

clean:
	rm -f ../$(MODULE)

.PHONY: all clean print
