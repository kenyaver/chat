all: server client

lib: check.o function.o command.o user.o timer.o offline.o onlineList.o protocol.o session.o

check.o:
	g++ -c include/check.cpp -o build/check.o

function.o:
	g++ -c include/function.cpp -o build/function.o

command.o:
	g++ -c include/command.cpp -o build/command.o

user.o:
	g++ -c include/user.cpp -o build/user.o

timer.o:
	g++ -c include/timer.cpp -o build/timer.o

offline.o:
	g++ -c include/offline.cpp -o build/offline.o

onlineList.o:
	g++ -c include/onlineList.cpp -o build/onlineList.o

protocol.o:
	g++ -c include/protocol.cpp -o build/protocol.o

session.o:
	g++ -c include/Session.cpp -o build/Session.o

server: check.o function.o command.o user.o timer.o onlineList.o offline.o protocol.o ession.o
	g++ build/check.o build/function.o build/command.o build/user.o build/timer.o build/onlineList.o build/offline.o build/protocol.o build/ession.o src/server.cpp -o build/server

client: check.o function.o command.o
	g++ build/check.o build/function.o build/command.o src/client.cpp -o build/client


clear:
	rm build/*
