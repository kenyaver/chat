#ifndef TALK_H
#define TALK_H

#include "aheader.h"
#include "struct.h"
#include "function.h"

void handleClient(Client clientic, char* user);
void writeFile(Client clientic, char* user);
void talkUser(Client client1, Client client2);
void talk(Client client1, Client client2, std::atomic<bool>& flag);
#endif