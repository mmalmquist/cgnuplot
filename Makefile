CWARNS=-Wall -Wextra -Wpedantic -Wconversion
CFLAGS=-std=gnu99 $(CWARNS)
CLIBS=-lm

.PHONY all:
all: cgnuplot

cgnuplot: test/testing.c src/cgnuplot.c src/cgpstack.c src/cgpfigure.c src/cgpentry.c src/cgppoint.c
	$(CC) -o $@ $^ $(CFLAGS) -O2 $(CLIBS)

.PHONY clean:
clean:
	rm -f cgnuplot
