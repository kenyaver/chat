#ifndef TALK_H
#define TALK_H

#include "aheader.h"
#include "struct.h"
#include "function.h"
#include "check.h"

void handleClient(std::vector<Client>&& clients, Client&& clientic, char* user);
void writeFile(Client&& clientic, char* user);
void talk(Client&& client1, Client&& client2, std::atomic<int>&& flag);
#endif