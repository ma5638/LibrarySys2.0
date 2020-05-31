output: main.o UniLibrarySys2.o
	g++ main.o UniLibrarySys2.o -o output
UniLibrarySys.o: UniLibrarySys2.h UniLibrarySys2.cpp
	g++ -c UniLibrarySys2.cpp
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm *.o output
