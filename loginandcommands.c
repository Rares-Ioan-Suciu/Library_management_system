#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loginandcommands.h"
#include "person.h"


void parseCommandLine(int argc, char *argv[], char **name, char **surname) {

    //function that pasres the argumnets when the program in ran
    if(argc == 1)
    {
        printf("\033[1;31mInvalid usage. Please use login [NAME] [SURNAME].\n");
        exit(1);
    } // printing propers messaeges for different cases of failure
    if (argc != 4 || strcmp(argv[1], "login") != 0)
    {
        printf("\033[1;31mInvalid usage. Please use login [NAME] [SURNAME].\n");
        if(strcmp(argv[1], "login") != 0)
            printf("\033[1;31mDon't forget to put login in front of [NAME] [SURNAME].\n");
        if(argc != 4 )
            printf("\033[1;31mPlease provide 3 arguments: login, [NAME] and [SURNAME] .\n");
        exit(1);
    }

    *name = argv[2];
    *surname = argv[3]; // storing name and surname for creating the logged structure
}

int checkCredentials(const char *name, const char *surname) {
    //ckecking if a person is in the users file, i.e. if the have an account
    FILE *file = fopen("users.csv", "r");
    if (file == NULL) {
        printf("Error opening users file.\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *tok = strtok(line, ",");
        if (strcmp(tok, name) == 0) {
            tok = strtok(NULL, ",");
            tok[strcspn(tok, "\n")] = '\0';
            if (strcmp(tok, surname) == 0) {
                fclose(file);
                return 1; // returnig if the exist in the file
            }
        }
    }

    fclose(file);
    return 0; // return 0 if they are not in the file
}

void handleLogin(const char *name, const char *surname, struct Person **logged) {
    if (checkCredentials(name, surname)) { // checking if the person already has an account made
        printf("\033[1;32mLogin successful! Welcome, %s %s.\n", name, surname);
        *logged = create_person(name, surname);
    } else { // if not ask user if he wants an account
        printf("\033[1;31mPerson non-registered. Would you like to make an account for %s %s? (yes/no): ", name, surname);
        char response[10];
        fgets(response, sizeof(response), stdin);
        response[strcspn(response, "\n")] = '\0';

        if (strcmp(response, "yes") == 0 || strcmp(response, "y") == 0 || strcmp(response, "Y")==0 || strcmp(response, "Yes") == 0) {
            struct Person newPerson;
            initPerson(&newPerson, name, surname);
            printf("\033[1;32mRegistration successful! Welcome, %s %s.\n", name, surname);
            *logged = create_person(name, surname);  // create new person and it to the file if they want an account
        }
        else  if (strcmp(response, "no") == 0 || strcmp(response, "n") == 0 || strcmp(response, "N") == 0 || strcmp(response, "No") == 0)
        {
            printf("\033[1;32mAlright! Have a good day, %s %s.\n", name, surname);
            exit(1);  // exit program if they don't want an account
        }
        else
        {
            printf("\033[1;32mPlease answer with either a yes or a no, %s %s.\n", name, surname);
            handleLogin(name, surname, &logged);
        }
    }
}


