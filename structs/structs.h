#ifndef STRUCTS_H
#define STRUCTS_H

#define SALES_FILE "./data/sales.csv"
#define CUSTOMERS_FILE "./data/customers.csv"
#define ITEMS_FILE "./data/items.csv"

typedef struct {
    int id;
    char* name;
    double price;
} Item;

typedef struct {
    int id;
    char* name;
} Customer;

#endif