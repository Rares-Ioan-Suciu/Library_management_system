//
// Created by rares655 on 4/10/24.
//

#ifndef LIBRARY_MANAGEMENT_SYSTEM_MENU_H
#define LIBRARY_MANAGEMENT_SYSTEM_MENU_H
#include "library.h"
#include "person.h"

void handleMenuChoice(char *command, struct Person *logged);
void printMenuAndGetChoice(char *command, int size);
void printBanner();

#endif //LIBRARY_MANAGEMENT_SYSTEM_MENU_H
