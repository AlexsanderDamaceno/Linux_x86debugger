all: debugger.cpp main.cpp library/linenoise/linenoise.c breakpoint.cpp
	g++   -c -o   main.o main.cpp
	g++   -c -o   registers.o registers.cpp
	g++   -c -o   debugger.o debugger.cpp
	g++   -c -o   breakpoint.o breakpoint.cpp
	gcc   -c -o linenoise.o library/linenoise/linenoise.c
	g++   -o dbg main.o debugger.o linenoise.o  registers.o breakpoint.o
	g++   -no-pie -g -O0 -o example examples_executables/exemplo.cpp

clean:
	  rm -f *.o
		rm dbg  
