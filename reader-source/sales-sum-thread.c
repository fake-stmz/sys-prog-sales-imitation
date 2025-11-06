#include "reader.h"

void calculate_sum() {
    pthread_rwlock_rdlock(&sales_rwlock);

    for (int i = 0; i < report_data.sales_count; i++) {
        report_data.sales_sum += sales[i].quantity;
        report_data.profit += sales[i].quantity * items[sales[i].item_id - 1].price;
    }

    pthread_rwlock_unlock(&sales_rwlock);

    printf ("Сумма продаж: %d, прибыль: %d\n", report_data.sales_sum, report_data.profit);
}

void* sales_sum_thread(void* arg) {
    
    while (working) {
        pthread_mutex_lock(&sum_mutex);
        calculate_sum();
    }

    return NULL;
}