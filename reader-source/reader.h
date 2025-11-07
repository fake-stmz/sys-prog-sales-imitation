#ifndef READER_H
#define READER_H

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "../structs/structs.h"
#include "../logging/logging.h"

#define REPORT_FILE "./data/report.txt"

extern sale_t* sales;

extern volatile int working;
extern pthread_rwlock_t sales_rwlock;
extern pthread_mutex_t read_mutex;
extern pthread_mutex_t sum_mutex;
extern pthread_cond_t sum_cond;
extern pthread_mutex_t top_mutex;
extern pthread_mutex_t customers_mutex;
extern pthread_mutex_t trendline_mutex;
extern pthread_mutex_t report_mutex;

typedef struct {
    item_t item;
    int count;
    int rank;
} top_entry_t;

typedef struct {
    char* date;
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
    customer_t customer;
    int total_spending;
} customer_spending_t;

void* read_sales_thread(void* arg);

void* sales_sum_thread(void* arg);

void* items_top_thread(void* arg);
void print_entries(top_entry_t* entries, FILE* to);

void* customer_report_thread(void* arg);

void* trendline_thread(void* arg);

void* report_thread(void* arg);

#endif