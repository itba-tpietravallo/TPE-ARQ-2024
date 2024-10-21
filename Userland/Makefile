include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: libc libsys sampleCodeModule sampleDataModule

libc:
	cd libc; make all

libsys:
	cd libsys; make all

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd libc; make clean
	cd libsys; make clean
	rm -rf *.bin *.o *.so *.a

.PHONY: libc libsys sampleCodeModule all clean
