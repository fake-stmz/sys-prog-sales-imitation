#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "../structs/structs.h"

typedef struct {
    int sale_id;
    char date_time[20];
    int customer_id;
    int item_id;
    int quantity;
} Sale;

extern Sale* sales;

extern int working;
extern sem_t read_sem;

void* read_sales_thread(void* arg);

void* sales_sum_thread(void* arg);

void* items_top_thread(void* arg);

void* customer_report_thread(void* arg);

void* trendline_thread(void* arg);

void* report_thread(void* arg);

#endif