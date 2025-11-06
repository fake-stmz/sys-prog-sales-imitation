#include "reader.h"

sale_t* sales = NULL;
volatile int working = 1;
pthread_rwlock_t sales_rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t read_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t top_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t customers_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trendline_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t report_mutex = PTHREAD_MUTEX_INITIALIZER;
report_data_t report_data;
item_t items[12];
customer_t customers[4];

void init_report_data(report_data_t* report_data) {
    report_data->date = NULL;
    report_data->sales_count = 0;
    report_data->sales_sum = 0;
    report_data->profit = 0;
    report_data->most_popular_items = NULL;
    report_data->least_popular_items = NULL;
}

int main() {
    init_report_data(&report_data);
    read_items(items);
    read_customers(customers);

    pthread_mutex_init(&read_mutex, NULL);
    pthread_rwlock_init(&sales_rwlock, NULL);
    pthread_mutex_init(&sum_mutex, NULL);
    pthread_mutex_init(&top_mutex, NULL);
    pthread_mutex_init(&customers_mutex, NULL);
    pthread_mutex_init(&trendline_mutex, NULL);
    pthread_mutex_init(&report_mutex, NULL);

    pthread_mutex_lock(&read_mutex);
    pthread_mutex_lock(&sum_mutex);
    pthread_mutex_lock(&top_mutex);
    pthread_mutex_lock(&customers_mutex);
    pthread_mutex_lock(&trendline_mutex);
    pthread_mutex_lock(&report_mutex);

    printf("=== Анализ продаж ===\n");

    pthread_t read_thread, sum_thread, top_thread, customer_thread, trend_thread, rep_thread;
    int treshold = 0;

    pthread_create(&read_thread, NULL, read_sales_thread, NULL);
    pthread_create(&sum_thread, NULL, sales_sum_thread, NULL);
    pthread_create(&top_thread, NULL, items_top_thread, NULL);
    pthread_create(&customer_thread, NULL, customer_report_thread, &treshold);
    pthread_create(&trend_thread, NULL, trendline_thread, NULL);
    pthread_create(&rep_thread, NULL, report_thread, NULL);

    while (working) {
        sleep(3);

        printf("Выберите действие:\n");
        printf("1 - Обновить данные\n");
        printf("2 - Вывести сумму продаж\n");
        printf("3 - Вывести популярные и непопулярные товары\n");
        printf("4 - Вывести активных покупателей\n");
        printf("5 - Вывести прогноз 3-х следующих продаж\n");
        printf("6 - Сгенерировать отчет\n");
        printf("0 - выйти\n");
        printf(">>>");
        int choice;
        scanf("%d", &choice);
        
        if (choice == 1) {
            pthread_mutex_unlock(&read_mutex);
        } else if (choice == 2) {
            pthread_mutex_unlock(&sum_mutex);
        } else if (choice == 3) {
            pthread_mutex_unlock(&top_mutex);
        } else if (choice == 4) {
            printf("Введите порог активности: ");
            scanf("%d", &treshold);
            pthread_mutex_unlock(&customers_mutex);
        } else if (choice == 5) {
            pthread_mutex_unlock(&trendline_mutex);
        } else if (choice == 6) {
            pthread_mutex_unlock(&sum_mutex);
            pthread_mutex_unlock(&top_mutex);

            sleep(2);

            pthread_mutex_unlock(&report_mutex);
        } else if (choice == 0) {
            working = 0;
            pthread_mutex_unlock(&read_mutex);
            pthread_mutex_unlock(&sum_mutex);
            pthread_mutex_unlock(&top_mutex);
            pthread_mutex_unlock(&customers_mutex);
            pthread_mutex_unlock(&trendline_mutex);
            pthread_mutex_unlock(&report_mutex);
        }
    }

    return 0;
}