#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loginandcommands.h"
#include "person.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    char command[100] = {""}; // to store command
    struct Person *logged = NULL; // to retain what person we are working with


    char *name, *surname;

    // Parsing function for the command-line arguments
    parseCommandLine(argc, argv, &name, &surname);

    // banner for the program
    printBanner();

    // Attempt to log in the user using the provided name and surname
    handleLogin(name, surname, &logged);

    // Main loop of the program for handling user input and menu choices
    while (1) {
        // Call the function to handle user menu choices
        handleMenuChoice(command, logged);
    }
    return 0;
}
