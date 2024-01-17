all: server client

app: server client

lib: check.o function.o class.o

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o

class.o:
	g++ -c libs/class.cpp -o build/class.o

server.o:
	g++ - c src/server.cpp -o build/server.o

client.o:
	g++ -c src/client.cpp -o build/client.o

server: check.o function.o class.o
	g++ build/check.o build/function.o build/class.o src/server.cpp -o build/server

client: check.o function.o class.o
	g++ build/check.o build/function.o build/class.o src/client.cpp -o build/client


clear:
	rm build/*
