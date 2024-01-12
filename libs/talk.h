#ifndef TALK_H
#define TALK_H

#include "aheader.h"
#include "struct.h"
#include "function.h"
#include "check.h"

char* get(Client& client, char* buffer , int id);
int put(char* buffer, Client& client);

void handleClient(std::vector<Client*>& clients, Client* client);

void talk(Client* client1, Client* client2);
int writeFile(char* sender, char* recver, char* buffer);
#endif