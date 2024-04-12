#include <stdio.h>
#include <string.h>
#include "library.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50


void getBookDetails(char *title, char *author){

    // function used in different places to read book inforamtion
    printf("Enter book title: ");
    fgets(title, MAX_TITLE_LENGTH, stdin);
    title[strcspn(title, "\n")] = '\0';

    printf("Enter author: ");
    fgets(author, MAX_AUTHOR_LENGTH, stdin);
    author[strcspn(author, "\n")] = '\0';
}

void trim_trailing_spaces(char *str) {
    int length = strlen(str); // removing any space characters at the end of string
    while (length > 0 && str[length - 1] == ' ') {
        str[length - 1] = '\0';
        length--;
    }
}

void add_book(const char title[MAX_TITLE_LENGTH], const char author[MAX_AUTHOR_LENGTH], int quantity) {
    FILE *file = fopen("library_stock.csv", "r+");
    if (file == NULL) {
        printf("Error opening library_stock.csv for reading and writing.\n");
        return;
    } //opening the stock file

    char line[256];
    char existing_author[MAX_AUTHOR_LENGTH], existing_title[MAX_TITLE_LENGTH];
    int existing_quantity;
    long int position;
    int bookFound = 0;

    while (fgets(line, sizeof(line), file) != NULL) { // going through each line of the file to see if the books is already in stock
        sscanf(line, "%[^,],%[^,],%d", existing_author, existing_title, &existing_quantity);
        if (strcmp(existing_author, author) == 0 && strcmp(existing_title, title) == 0) {

            quantity += existing_quantity;

            position = ftell(file);

            fseek(file, position - strlen(line), SEEK_SET);

            fprintf(file, "%s,%s,%d\n", existing_author, existing_title, quantity);

            printf("Quantity of the book updated in the library_stock.csv.\n");   // if it's stock we just update the quantity in the file
            bookFound = 1;
            break;
        }
    }

    fclose(file);

    if (!bookFound) {

        file = fopen("library_stock.csv", "a");
        if (file == NULL) {
            printf("Error opening library_stock.csv for appending.\n");
            return;
        }

        fprintf(file, "%s,%s,%d\n", author, title, quantity);  // if it's not in stock, we add it to the end of the file, for future use
        printf("Book added to the library_stock.csv.\n");
        fclose(file);
    }
}

int check_book(const char *title, const char *author, int quantity) { // function to check if a book is in stock, used when borrowing
    FILE *library_stock_fp = fopen(LIBRARY_STOCK_FILE, "r");
    if (library_stock_fp == NULL) {
        printf("Error: Cannot open library stock file.\n");
        return 0;
    }

    char line[256];
    char current_title[MAX_TITLE_LENGTH];
    char current_author[MAX_AUTHOR_LENGTH];
    int current_quantity;
    int book_found = 0;

    while (fgets(line, sizeof(line), library_stock_fp) != NULL) {
        sscanf(line, "%[^,],%[^,],%d", current_author, current_title, &current_quantity);
        if (strcmp(current_title, title) == 0 && strcmp(current_author, author) == 0) {
            if (current_quantity >= quantity) { // Checking if we have in the file enough books for the demaned quantiy
                book_found = 1;
            } else {
                printf("Error: Not enough copies of '%s' by %s available for borrowing.\n", title, author);
            }
            break;
        }
    }

    fclose(library_stock_fp);

    if (!book_found) {
        printf("Error: Book '%s' by %s is not in the library.\n", title, author); // prints this when the book is not in stock at all
    }

    return book_found;
}

void borrow_book(const char *title, const char *author, int quantity, struct Person *borrower) {
    if (!check_book(title, author, quantity)) { // check if the books are borrowable
        return;
    }

    FILE *borrower_fp = fopen(borrower->file, "a");
    if (borrower_fp == NULL) {
        printf("Error: Cannot open borrower file.\n");
        return;
    }

    fprintf(borrower_fp, "%s,%s,%d\n", author, title, quantity); // writes the book to the borrower's file

    fclose(borrower_fp);

    add_book(title, author, -quantity);  // updates stock

    printf("Book '%s' by %s (Quantity: %d) borrowed successfully by %s %s.Thank you.\n", title, author, quantity, borrower->name, borrower->surname);
}

void searchBook(const char *query) {
    FILE *library_stock_fp = fopen(LIBRARY_STOCK_FILE, "r");
    if (library_stock_fp == NULL) {
        printf("Error: Unable to open library stock file.\n");
        return;
    }

    printf("Search Results:\n");

    char line[256];
    int found = 0;
    int i = 0;
    while (fgets(line, sizeof(line), library_stock_fp) != NULL) {
        if(i) {
            char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH];
            int quantity;
            sscanf(line, "%[^,],%[^,],%d", author, title, &quantity);

            // Case where  the query is shorter than both title and author
            if (strlen(query) <= strlen(title) && strlen(query) <= strlen(author)) {
                if (strstr(title, query) != NULL || strstr(author, query) != NULL) {
                    printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
                    found = 1;
                }
            }
                // Case where  query is longer than both title and author
            else if (strlen(query) > strlen(title) && strlen(query) > strlen(author)) {
                if (strstr(query, title) != NULL && strstr(query, author) != NULL) {
                    printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
                    found = 1;
                }
            }
                // Case where  query is longer than title but shorter than author
            else if (strlen(query) > strlen(title) && strlen(query) < strlen(author)) {
                if (strstr(query, title) != NULL || strstr(author, query) != NULL) {
                    printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
                    found = 1;
                }
            }
                // Case where query is shorter than title but longer than author
            else if (strlen(query) < strlen(title) && strlen(query) > strlen(author)) {
                if (strstr(title, query) != NULL || strstr(query, author) != NULL) {
                    printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
                    found = 1;
                }
            }
            else if (strcmp(query, title) == 0 || strcmp(query, author) == 0) {
                printf("Author: %s, Title: %s, Quantity: %d\n", author, title, quantity);
                found = 1;
            }
        }
        i++;
    }

    if (!found) {
        printf("No matching books found.\n");
    }

    fclose(library_stock_fp);
}


void returnbook(char *title, char *author, int quantity, struct Person *logged) {
    FILE *borrower_fp = fopen(logged->file, "r+");
    if (borrower_fp == NULL) {
        printf("Error: Cannot open borrower file.\n");
        return;
    }

    FILE *temp_fp = tmpfile();
    if (temp_fp == NULL) {
        printf("Error: Cannot create temporary file.\n");
        fclose(borrower_fp);
        return;
    }

    char line[300];
    int book_found = 0;
    int borrowed_quantity = 0;

    trim_trailing_spaces(title);
    trim_trailing_spaces(author);

    while (fgets(line, sizeof(line), borrower_fp) != NULL) {
        char current_title[MAX_TITLE_LENGTH], current_author[MAX_AUTHOR_LENGTH];
        int current_quantity;

        sscanf(line, "%[^,],%[^,],%d", current_author, current_title, &current_quantity);
        if (strcmp(current_title, title) == 0 && strcmp(current_author, author) == 0) {
            book_found = 1;
            borrowed_quantity = current_quantity;
            if (current_quantity > quantity) {
                fprintf(temp_fp, "%s,%s,%d\n", current_author, current_title, current_quantity - quantity);
            } else if (current_quantity == quantity) {
                // Book fully returned, do not write it to temp file
            } else {
                printf("Error: Trying to return more books than borrowed.\n");
                fclose(borrower_fp);
                fclose(temp_fp);
                return;
            }
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    fclose(borrower_fp);
    rewind(temp_fp);

    borrower_fp = fopen(logged->file, "w");
    if (borrower_fp == NULL) {
        printf("Error: Cannot open borrower file for writing.\n");
        fclose(temp_fp);
        return;
    }

    while (fgets(line, sizeof(line), temp_fp) != NULL) {
        fprintf(borrower_fp, "%s", line);
    }

    fclose(borrower_fp);
    fclose(temp_fp);

    remove(tempnam(NULL, NULL));

    if (book_found) {
        add_book(title, author, quantity); // Increase the quantity of the book in the library stock
        printf("Book returned successfully.\n");
    } else {
        printf("Error: Book not found in borrower's file.\n");
    }
}

void create_loan_file(const char *name, const char *surname, const char titles[][MAX_TITLE_LENGTH], const char authors[][MAX_AUTHOR_LENGTH], const int quantities[], int num_books) {
    // function used to create a file for each loan made, contait information about who did it, when and what they borrowd, for administrative purposes
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);
    char loan_filename[256];
    snprintf(loan_filename, sizeof(loan_filename), "loan_%s_%s_%02d_%02d.txt", name, surname, time_info->tm_hour, time_info->tm_mday); // creating said file

    FILE *loan_fp = fopen(loan_filename, "w");
    if (loan_fp == NULL) {
        printf("Error: Cannot create loan file.\n");
        return;
    }

    fprintf(loan_fp, "%s %s\n", name, surname);

    for (int i = 0; i < num_books; i++) {
        fprintf(loan_fp, "Title: %s, Author: %s, Quantity: %d\n", titles[i], authors[i], quantities[i]); // writing all borrwoed books there
    }

    fclose(loan_fp);
}

