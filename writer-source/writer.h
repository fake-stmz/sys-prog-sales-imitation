#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define OUTPUT_FILE "data/sales.csv"
#define CUSTOMERS_FILE "data/customers.csv"
#define ITEMS_FILE "data/items.csv"

typedef struct {
    int id;
    char* name;
    double price;
} Item;

typedef struct {
    int id;
    char* name;
} Customer;

typedef struct {
    Item* items;
    Customer* customers;
} WriterArgs;

int writing_active = 1;
int next_sale_id = 0;

void* writer_thread(void* arg);

#endif