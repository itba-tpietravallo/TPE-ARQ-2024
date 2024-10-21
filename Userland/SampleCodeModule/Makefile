include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c -L../ $(SOURCES) -l:libc.a -l:libsys.a -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
