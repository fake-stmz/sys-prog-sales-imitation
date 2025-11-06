#include "reader.h"

customer_spending_t customer_spendings[4];

void init_customer_spendings() {
    for (int i = 0; i < 4; i++) {
        customer_spendings[i].customer = customers[i];
        customer_spendings[i].total_spending = 0;
    }
}

void get_active_customers(int* treshold) {
    init_customer_spendings();

    pthread_rwlock_rdlock(&sales_rwlock);

    for (int i = 0; i < report_data.sales_count; i++) {
        customer_spendings[sales[i].customer_id - 1].total_spending += sales[i].quantity * items[sales[i].item_id - 1].price;
    }

    pthread_rwlock_unlock(&sales_rwlock);

    printf("Покупатели, набравшие товаров в количестве более %d:\n", *treshold);
    for (int i = 0; i < 4; i++) {
        if (customer_spendings[i].total_spending >= *treshold) {
            printf("%s - %d\n", customer_spendings[i].customer.name, customer_spendings[i].total_spending);
        }
    }
}

void* customer_report_thread(void* arg) {
    int* treshold = (int*)arg;

    while (working) {
        get_active_customers(treshold);
        pthread_mutex_lock(&customers_mutex);
    }

    return NULL;
}