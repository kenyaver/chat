#ifndef TALK_H
#define TALK_H

#include "aheader.h"
#include "struct.h"
#include "function.h"
#include "check.h"

int writeFile(char* sender, char* recver, char* buffer);
void talk(Client&& client1, Client&& client2);
#endif