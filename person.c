#include <stdio.h>
#include <string.h>
#include "person.h"

void initPerson(struct Person *person, const char *name, const char *surname) {
   // function to be used for the registration portion of a person
    char *path = getExecutableDirectory(); // get directory for later use
    if (path == NULL) {
        printf("Error: Unable to retrieve executable directory.\n");
        return;
    }
    strncpy(person->name, name, MAX_NAME_LENGTH - 1);
    person->name[MAX_NAME_LENGTH - 1] = '\0';  // storing name

    strncpy(person->surname, surname, MAX_SURNAME_LENGTH - 1);
    person->surname[MAX_SURNAME_LENGTH - 1] = '\0'; // stroring surname

    snprintf(person->file, sizeof(person->file), "%s/%s_%s.csv", path, name, surname);
    free(path);

    FILE *file = fopen(person->file, "w");
    if (file == NULL) {
        printf("Error creating file for %s %s.\n", name, surname);
        perror("fopen");
        return;
    }
    fprintf(file, "Name: %s,Surname: %s\n", name, surname);
    fclose(file); // creating the file for the all the loans

    // Appending the person's information to the users.csv file, so the library has it
    FILE *usersFile = fopen("users.csv", "a");
    if (usersFile == NULL) {
        printf("Error opening users file.\n");
        perror("fopen");
        return;
    }
    if (fprintf(usersFile, "%s,%s,%s\n", name, surname, person->file) < 0) {
        printf("Error writing to users file.\n");
        perror("fprintf");
        fclose(usersFile);
        return;
    }
    fclose(usersFile);

    // Print cool message with the person's name, surname, and the file created
    printf("Person %s %s initialized. File created: %s\n", name, surname, person->file);
}

struct Person* create_person(const char* name, const char* surname) {
     // function used to creat the actual person structure used in the program, used at each login
    struct Person* newPerson = malloc(sizeof(struct Person));
    if (newPerson == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }


    strncpy(newPerson->name, name, MAX_NAME_LENGTH - 1);
    newPerson->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(newPerson->surname, surname, MAX_SURNAME_LENGTH - 1);
    newPerson->surname[MAX_SURNAME_LENGTH - 1] = '\0';

    // Get the path of the directory
    char *path = getExecutableDirectory();
    if (path == NULL) {
        printf("Error: Unable to retrieve executable directory.\n");
        free(newPerson);
        return NULL;
    }

    // Construct the file path for the new person's data file
    snprintf(newPerson->file, sizeof(newPerson->file), "%s/%s_%s.csv", path, name, surname);
    free(path);

    return newPerson;
}

void printloans(struct Person *logged) {
    // Open the loan file associated with the logged-in user
    FILE *loan_file = fopen(logged->file, "r");
    if (loan_file == NULL) {
        printf("Error: Unable to open loan file for %s %s.\n", logged->name, logged->surname);
        return;
    }

    // Print a nice header for borrowed books
    printf("Borrowed Books for %s %s:\n", logged->name, logged->surname);

    // Next-up, code to read and print each line from the loan file
    char line[256];
    char i = 0;
    while (fgets(line, sizeof(line), loan_file) != NULL) {
        if (i != 0) {
            char author[MAX_AUTHOR_LENGTH], title[MAX_TITLE_LENGTH];
            int quantity;

            // Extracting author, title, and quantity from the line
            sscanf(line, "%[^,],%[^,],%d", author, title, &quantity);

            printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
        }
        i++;
    }

    if (i == 1) {
        printf("No books loaned, choose command 2 to borrow some.\n");
    }

    fclose(loan_file);
}