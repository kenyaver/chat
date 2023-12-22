all: server

server:
	gcc src/server.cpp libs/check.cpp libs/function.cpp libs/struct.cpp libs/talk.cpp -o build/server

client:
	gcc src/client.cpp libs/check.cpp libs/functions.c libs/struct.cpp libs/talk.cpp -o build/client

clear:
	rm server client