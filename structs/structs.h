#ifndef STRUCTS_H
#define STRUCTS_H

#define SALES_FILE "./data/sales.csv"
#define CUSTOMERS_FILE "./data/customers.csv"
#define ITEMS_FILE "./data/items.csv"

typedef struct {
    int id;
    char* name;
    double price;
} item_t;

typedef struct {
    int id;
    char* name;
} customer_t;

typedef struct {
    int sale_id;
    char date_time[20];
    int customer_id;
    int item_id;
    int quantity;
} sale_t;

void read_items(item_t* items);

void read_customers(customer_t* customers);

#endif