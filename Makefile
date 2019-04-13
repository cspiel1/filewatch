all: filewatch openrw

filewatch: filewatch.c
	g++ $< -o $@

openrw: openrw.c
	g++ $< -o $@

