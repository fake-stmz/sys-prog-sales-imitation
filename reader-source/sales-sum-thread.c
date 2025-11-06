#include "reader.h"

void calculate_sum() {
    for (int i = 0; i < report_data.sales_count; i++) {
        report_data.sales_sum += sales[i].quantity;
        report_data.profit += sales[i].quantity * items[sales[i].item_id - 1].price;
    }
}

void* sales_sum_thread(void* arg) {
    
    while (working) {
        calculate_sum();
        pthread_mutex_lock(&sum_mutex);
    }

    return NULL;
}