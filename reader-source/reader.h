#ifndef READER_H
#define READER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "../structs/structs.h"

extern sale_t* sales;

extern volatile int working;
extern pthread_rwlock_t sales_rwlock;
extern pthread_mutex_t read_mutex;
extern pthread_mutex_t sum_mutex;
extern pthread_mutex_t top_mutex;
extern pthread_mutex_t customers_mutex;

typedef struct {
    int sales_count;
    int sales_sum;
    int profit;
    top_entry_t* most_popular_items;
    top_entry_t* least_popular_items;
} report_data_t;

extern report_data_t report_data;

extern item_t items[12];    
extern customer_t customers[4];

typedef struct {
    item_t item;
    int count;
    int rank;
} top_entry_t;

typedef struct {
    customer_t customer;
    int total_spending;
} customer_spending_t;

void* read_sales_thread(void* arg);

void* sales_sum_thread(void* arg);

void* items_top_thread(void* arg);

void* customer_report_thread(void* arg);

void* trendline_thread(void* arg);

void* report_thread(void* arg);

#endif