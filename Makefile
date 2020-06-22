
main.exe: main.o 
	g++ -std=c++0x main.o -o main.exe

main.o: main.cpp coda.h
	g++ -std=c++0x -c main.cpp -o main.o

.PHONY: clean

clean:
	rm *.exe *.o