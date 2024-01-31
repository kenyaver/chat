all: server client

lib: check.o function.o

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o


User.o:
	g++ -c libs/User.cpp -o build/User.o

protocol.o:
	g++ -c libs/protocol.cpp -o build/protocol.o

Session.o:
	g++ -c libs/Session.cpp -o build/Session.o



server: check.o function.o User.o protocol.o Session.o
	g++ build/check.o build/function.o build/User.o build/protocol.o build/Session.o src/server.cpp -o build/server

client: check.o function.o User.o protocol.o Session.o
	g++ build/check.o build/function.o build/User.o build/protocol.o build/Session.o src/client.cpp -o build/client


clear:
	rm build/*
