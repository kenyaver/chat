all: server

server:
	g++ src/server.cpp libs/check.cpp libs/function.cpp libs/struct.cpp libs/talk.cpp -o build/server

client:
	g++ src/client.cpp libs/check.cpp libs/function.cpp libs/struct.cpp libs/talk.cpp -o build/client

chatS:
	g++ src/chat.cpp libs/check.cpp libs/function.cpp -o build/chatS

chatC:
	g++ src/clientChat.cpp libs/check.cpp libs/function.cpp -o build/chatC

clear:
	rm server client