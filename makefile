all: server client

lib: check.o function.o Client.o Reader.o Talk.o

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o


Reader.o:
	g++ -c libs/Reader.cpp -o build/Reader.o

Client.o:
	g++ -c libs/Client.cpp -o build/Client.o

Talk.o:
	g++ -c libs/Talk.cpp -o build/Talk.o



server: check.o function.o Client.o Reader.o Talk.o
	g++ build/check.o build/function.o build/Client.o build/Reader.o build/Talk.o src/server.cpp -o build/server

client: check.o function.o Client.o Reader.o Talk.o
	g++ build/check.o build/function.o build/Client.o build/Reader.o build/Talk.o src/client.cpp -o build/client


clear:
	rm build/*
