#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "../structs/structs.h"

typedef struct {
    item_t* items;
    customer_t* customers;
} writer_args_t;

extern volatile int writing_active;
extern volatile int next_sale_id;

void* writer_thread(void* arg);

extern pthread_mutex_t sale_mutex;

#endif