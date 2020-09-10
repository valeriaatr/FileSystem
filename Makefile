CC:=gcc
CFLAGS:=-g -Wall -Werror 

TESTFILES := $(wildcard apps/test*.c) 
$(info TESTFILES are $(TESTFILES))

TESTS := $(TESTFILES:apps/%.c=%)
$(info TESTS are $(TESTS))

all: $(TESTS) disk.o File.o

test%: apps/test%.c File.o
	$(CC) $(CFLAGS) -o apps/$@ $^

disk.o: disk/disk.c disk/disk.h
	$(CC) $(CFLAGS) -c -o $@ $<

File.o: io/File.c io/File.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
index_bits.o: io/index_bits.c io/index_bits.h
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf *.o
	find apps -type f -not -name '*.c' -print0 | xargs -0 rm --
