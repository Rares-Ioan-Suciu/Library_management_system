#include <stdlib.h>
#include "loginandcommands.h"
#include "path.h"
#ifndef LIBRARY_MANAGEMENT_SYSTEM_PERSON_H
#define LIBRARY_MANAGEMENT_SYSTEM_PERSON_H

#define MAX_NAME_LENGTH 50
#define MAX_SURNAME_LENGTH 50
#define MAX_BORROWED_BOOKS 10
#define MAX_BOOK_TITLE_LENGTH 100
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50

struct Person {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];
    char file[MAX_NAME_LENGTH + MAX_SURNAME_LENGTH + 5]; // Assuming format: name_surname.csv
};

void initPerson(struct Person *person, const char *name, const char *surname);
struct Person* create_person(const char* name, const char* surname);
void printloans(struct Person *logged);

#endif //LIBRARY_MANAGEMENT_SYSTEM_PERSON_H
