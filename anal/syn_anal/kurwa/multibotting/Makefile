all: target

target: kurwa.o module.o lex.o syn_kur.o
	g++ -Wall -g kurwa.o module.o lex.o syn_kur.o -o execfile

kurwa.o: kurwa.cpp
	g++ -Wall -g -c kurwa.cpp

module.o: module.cpp
	g++ -Wall -g -c module.cpp

lex.o: lex.cpp
	g++ -Wall -g -c lex.cpp

syn_kur.o: syn_kur.cpp
	g++ -Wall -g -c syn_kur.cpp

clean:
	rm -rf *.o
