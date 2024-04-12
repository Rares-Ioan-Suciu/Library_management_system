//
// Created by rares655 on 3/29/24.
//

#ifndef LIBRARY_MANAGEMENT_SYSTEM_LIBRARY_H
#define LIBRARY_MANAGEMENT_SYSTEM_LIBRARY_H

#include "person.h"
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define LIBRARY_STOCK_FILE "library_stock.csv"

typedef struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int Quantity;
}Book;

void getBookDetails(char *title, char *author);
void add_book(const char title[MAX_TITLE_LENGTH], const char author[MAX_AUTHOR_LENGTH], int quantity);
int check_book(const char *title, const char *author, int quantity);
void borrow_book(const char *title, const char *author, int quantity, struct Person* borrower);
void searchBook(const char *query);
void returnbook(char *title, char *author, int quantity, struct Person *logged);
void create_loan_file(const char *name, const char *surname, const char titles[][MAX_TITLE_LENGTH], const char authors[][MAX_AUTHOR_LENGTH], const int quantities[], int num_books);
void trim_trailing_spaces(char *str);

#endif //LIBRARY_MANAGEMENT_SYSTEM_LIBRARY_H
