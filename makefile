all: server client

lib: check.o command.o user.o timer.o offline.o onlineList.o protocol.o session.o

check.o:
	g++ -c include/check.cpp -o build/include/check.o

loger.o:
	g++ -c include/loger.cpp -o build/include/loger.o

command.o:
	g++ -c include/command.cpp -o build/include/command.o

user.o:
	g++ -c include/user.cpp -o build/include/user.o

unconfirm.o:
	g++ -c include/unconfirm.cpp -o build/include/unconfirm.o

timer.o:
	g++ -c include/timer.cpp -o build/include/timer.o

offline.o:
	g++ -c include/offline.cpp -o build/include/offline.o

onlineList.o:
	g++ -c include/onlineList.cpp -o build/include/onlineList.o

protocol.o:
	g++ -c include/protocol.cpp -o build/include/protocol.o

session.o:
	g++ -c include/session.cpp -o build/include/session.o

server: check.o command.o user.o unconfirm.o timer.o onlineList.o offline.o protocol.o session.o
	g++ -g build/include/check.o build/include/command.o build/include/user.o build/include/unconfirm.o build/include/timer.o build/include/onlineList.o build/include/offline.o build/include/protocol.o build/include/session.o src/server.cpp -o build/server

client: check.o command.o
	g++ -g build/include/check.o build/include/command.o src/client.cpp -o build/client


clear:
	rm -r build/*
