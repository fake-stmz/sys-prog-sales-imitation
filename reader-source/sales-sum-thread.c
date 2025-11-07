#include "reader.h"

void calculate_sum() {
    pthread_rwlock_rdlock(&sales_rwlock);

    report_data.sales_sum = 0;
    report_data.profit = 0;

    print_log("reader - sales_sum_thread", "Высчитывание суммы продаж...");
    for (int i = 0; i < report_data.sales_count; i++) {
        report_data.sales_sum += sales[i].quantity;
        report_data.profit += sales[i].quantity * items[sales[i].item_id - 1].price;
    }
    print_log("reader - sales_sum_thread", "Сумма продаж успешно высчитана.");

    pthread_rwlock_unlock(&sales_rwlock);

    print_log("reader - sales_sum_thread", "Вывод суммы продаж...");
    printf ("Сумма продаж: %d, прибыль: %d\n", report_data.sales_sum, report_data.profit);
}

void* sales_sum_thread(void* arg) {
    print_log("reader - sales_sum_thread", "Начало работы.");
    pthread_mutex_lock(&sum_mutex);

    while (working) {
        print_log("reader - sales_sum_thread", "Получение суммы продаж...");
        calculate_sum();
        pthread_mutex_lock(&sum_mutex);
    }

    print_log("reader - sales_sum_thread", "Завершение работы.");
    return NULL;
}