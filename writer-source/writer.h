#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "../structs/structs.h"

#define OUTPUT_FILE "./data/sales.csv"
#define CUSTOMERS_FILE "./data/customers.csv"
#define ITEMS_FILE "./data/items.csv"

typedef struct {
    Item* items;
    Customer* customers;
} WriterArgs;

extern int writing_active;
extern int next_sale_id;

void* writer_thread(void* arg);

extern pthread_mutex_t sale_mutex;

#endif