//
// Created by rares655 on 3/29/24.
//

#ifndef LIBRARY_MANAGEMENT_SYSTEM_LOGINANDCOMMANDS_H
#define LIBRARY_MANAGEMENT_SYSTEM_LOGINANDCOMMANDS_H
#include "person.h"



int checkCredentials(const char *name, const char *surname);
void parseCommandLine(int argc, char *argv[], char **name, char **surname);
void handleLogin(const char *name, const char *surname, struct Person **logged);

#endif //LIBRARY_MANAGEMENT_SYSTEM_LOGINANDCOMMANDS_H
