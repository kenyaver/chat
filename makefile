all: check.o function.o class.o cserver cclient

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o

class.o:
	g++ -c srcc/class.cpp -o build/class.o

fileSchat:
	g++ src/fileSchat.cpp build/check.o build/function.o build/struct.o build/talk.o -o build/fileSchat

cserver:
	g++ srcc/server.cpp build/check.o build/function.o build/class.o -o build/server

cclient:
	g++ srcc/client.cpp build/check.o build/function.o build/class.o -p build/client

server:
	g++ src/server.cpp build/check.o build/function.o build/struct.o build/talk.o -o build/schat

client:
	g++ src/client.cpp build/check.o build/function.o build/struct.o build/talk.o -o build/cchat


clear:
	rm build/server build/client

clearLibs:
	rm build/check.o build/function.o