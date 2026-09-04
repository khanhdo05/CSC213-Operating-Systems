all: partA partB

partA: partA.c
	clang --std=c11 -g -o partA partA.c

partB: partB.c
	clang --std=c11 -g -o partB partB.c

clean:
	rm -f partA partB

.PHONY: all clean
