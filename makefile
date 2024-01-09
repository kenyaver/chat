all: server client

libs: check.o function.o struct.o talk.o

check.o:
	g++ -c libs/check.cpp -o build/check.o

function.o:
	g++ -c libs/function.cpp -o build/function.o

struct.o:
	g++ -c libs/struct.cpp -o build/struct.o

talk.o:
	g++ -c libs/talk.cpp -o build/talk.o

server:
	g++ src/server.cpp build/check.o build/function.o build/struct.o build/talk.o -o build/server

client:
	g++ src/client.cpp build/check.o build/function.o build/struct.o build/talk.o -o build/client

chatS:
	g++ src/chat.cpp libs/check.cpp libs/function.cpp -o build/chatS

chatC:
	g++ src/clientChat.cpp libs/check.cpp libs/function.cpp -o build/chatC

clear:
	rm build/server build/client