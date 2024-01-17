all: server client

app: server client

lib: check.o function.o class.o

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o

class.o:
	g++ -c libs/class.cpp -o build/class.o


server: check.o function.o class.o
	g++ src/server.cpp build/check.o build/function.o build/class.o -o build/server

client: check.o function.o class.o
	g++ src/client.cpp build/check.o build/function.o build/class.o -p build/client


clear:
	rm build/*
