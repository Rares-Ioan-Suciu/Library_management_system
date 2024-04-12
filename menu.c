#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "person.h"
#include "library.h"
#include <time.h>
char title[MAX_TITLE_LENGTH];
char author[MAX_AUTHOR_LENGTH];

void printBanner() {
     // Banner soley in the hope of design
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    int width = 50;

    // Print banner
    printf("\033[1;36m");
    printf("**************************************************\n");
    printf("*           Library Management System            *\n");
    printf("**************************************************\n");
    printf("*%*s%02d:%02d:%02d %02d-%02d-%d%*s   *\n",
           (width - 23) / 2, "", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
           timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, (width - 23) / 2, "");
    printf("**************************************************\n\n");
    printf("\033[0m");
}

void printMenuAndGetChoice(char *command, int size)
{
    //prints the menu and stores the command in a variable
    printf("\033[1;33m\n");
    printf("**************************************************\n");
    printf("*            Library Management System           *\n");
    printf("**************************************************\n");
    printf("*           What would you like to do ?          *\n");
    printf("**************************************************\n");
    printf("*                1. Search Book                  *\n");
    printf("*                2. Borrow Books                 *\n");
    printf("*                3. Print All Loans              *\n");
    printf("*                4. Return Books                 *\n");
    printf("*                5. Donate Books                 *\n");
    printf("*                6. Clean Screen                 *\n");
    printf("*                7. Exit                         *\n");
    printf("**************************************************\n");
    printf("Enter your choice: ");
    fgets(command, size, stdin);


    command[strcspn(command, "\n")] = '\0';
}

void handleMenuChoice(char *command, struct Person *logged)
{
    while (1) {  // loops in the menu until eventual exit with command 7
        printMenuAndGetChoice(command, sizeof(command));

        if (strlen(command) == 0) {
            continue;  // for the case where  enter is pressed
        }
        if (strcmp(command, "1") == 0)   // first command, the search
        {
            char query[300];
            printf("Enter search query (title, author, or both) or press enter to see the entire stock: ");
            fgets(query, sizeof(query), stdin);
            query[strcspn(query, "\n")] = '\0';   // getting the query

            searchBook(query); // call specific function
            printf("Press Enter to continue...");
            getchar(); // Wait for user to press Enter, giving them a chance to see the results of their action
            printf("\n");
        }else if (strcmp(command, "2") == 0)  // command 2 borrow
        {
            int num_books;
            printf("Enter the number of different books you want to borrow: ");
            if (scanf("%d", &num_books) != 1 || num_books <= 0) {
                printf("Error: Please enter a valid positive integer.\n");
                return;
            } // getting the numbers of books being searched
            getchar();

            char titles[num_books][MAX_TITLE_LENGTH];
            char authors[num_books][MAX_AUTHOR_LENGTH];
            int quantities[num_books]; // Arrays to store information for each book

            for (int i = 0; i < num_books; i++)
            {
                printf("Reading informations for book %d: ", i + 1);
                getBookDetails(titles[i], authors[i]);

                printf("Enter the quantity of book %d: ", i + 1);
                scanf("%d", &quantities[i]);
                getchar();

                if (!check_book(titles[i], authors[i], quantities[i]))
                {
                    printf("Sorry, '%s' by %s is not available in the stock. Please try again with a different book.\n", titles[i], authors[i]);
                    return;
                }  // ckecking if all the books are available, getting out if not
            }
            create_loan_file(logged->name, logged->surname, titles, authors, quantities, num_books); // this creates a loan file to be used by the librarians, maybe
            for (int i = 0; i < num_books; i++)
            {
                borrow_book(titles[i], authors[i], quantities[i], logged); // Borrowing each book if they are in stock
            }

            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
        else if (strcmp(command, "3") == 0) {
            printloans(logged); // calling function for print
            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
        else if (strcmp(command, "4") == 0)
        {
            short int quantity;
            printf("\nLoans:\n");
            printloans(logged);  // firstly printing the loans

            getBookDetails(title, author);  // reading which book is to be retured
            printf("Enter the quantity of the book to return: ");
            scanf("%d", &quantity); // and quantity
            getchar();
            returnbook(title, author, quantity, logged); // functionn call

            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
        else if (strcmp(command, "5") == 0)
        {

            int quantity;
            getBookDetails(title, author); // getting which book is donated
            printf("Enter quantity: ");
            if (    scanf("%d", &quantity)!= 1 || quantity <= 0) {
                printf("Error: Please enter a valid positive integer.\n");
                return;
            } // checking number is okay
            getchar();
            add_book(title, author, quantity); // adding the book(s) to the library stock
            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
        else if (strcmp(command, "6") == 0) {
            system("clear"); // cleanung terminal, only for linux users
        }
        else if (strcmp(command, "7") == 0) {
            system("clear");
            printf("Have a great day. Exiting app...\n"); // exiting the program
            exit(1);
        }
        else {
            printf("Invalid choice. Please choose a task from 1-6 using one of the digits.\n"); // case for which the command doesn't exist
            return;
        }
    }
}
