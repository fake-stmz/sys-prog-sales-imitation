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
    report_data->most_popular_items = malloc(5 * sizeof(top_entry_t));
    report_data->least_popular_items = malloc(5 * sizeof(top_entry_t));
    
    if (report_data->most_popular_items) {
        memset(report_data->most_popular_items, 0, 5 * sizeof(top_entry_t));
    }
    if (report_data->least_popular_items) {
        memset(report_data->least_popular_items, 0, 5 * sizeof(top_entry_t));
    }
}

int main() {
    print_log("reader - main", "Начало работы.");

    print_log("reader - main", "Инициализация данных отчета...");
    init_report_data(&report_data);
    print_log("reader - main", "Данные отчета успешно инициализированы.");

    print_log("reader - main", "Чтение данных о клиентах и товарах...");
    read_items(items);
    read_customers(customers);
    print_log("reader - main", "Данные о клиентах и товарах успешно прочитаны.");

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

    print_log("reader - main", "Запуск потоков...");
    pthread_t read_thread, sum_thread, top_thread, customer_thread, trend_thread, rep_thread;
    int treshold = 0;

    pthread_create(&read_thread, NULL, read_sales_thread, NULL);
    pthread_create(&sum_thread, NULL, sales_sum_thread, NULL);
    pthread_create(&top_thread, NULL, items_top_thread, NULL);
    pthread_create(&customer_thread, NULL, customer_report_thread, &treshold);
    pthread_create(&trend_thread, NULL, trendline_thread, NULL);
    pthread_create(&rep_thread, NULL, report_thread, NULL);

    print_log("reader - main", "Потоки успешно запущены.");

    while (working) {
        sleep(1);
        print_log("reader - main", "Выбор действия...");

        printf("\nВыберите действие:\n");
        printf("1 - Обновить данные\n");
        printf("2 - Вывести сумму продаж\n");
        printf("3 - Вывести популярные и непопулярные товары\n");
        printf("4 - Вывести активных покупателей\n");
        printf("5 - Вывести прогноз 3-х следующих продаж\n");
        printf("6 - Сгенерировать отчет\n");
        printf("0 - Выйти\n");
        printf(">>> ");
        int choice;
        scanf("%d", &choice);

        print_log("reader - main", "Действие выбрано.");
        
        if (choice == 1) {
            print_log("reader - main", "Послан сигнал на обновление данных...");
            pthread_mutex_unlock(&read_mutex);
        } else if (choice == 2) {
            print_log("reader - main", "Послан сигнал на вывод суммы продаж...");
            pthread_mutex_unlock(&sum_mutex);
        } else if (choice == 3) {
            print_log("reader - main", "Послан сигнал на вывод популярных и непопулярных товаров...");
            pthread_mutex_unlock(&top_mutex);
            sleep(1);
        } else if (choice == 4) {
            print_log("reader - main", "Ожидание ввода порога активности...");
            printf("Введите порог активности: ");
            scanf("%d", &treshold);
            print_log("reader - main", "Послан сигнал на вывод активных покупателей...");
            pthread_mutex_unlock(&customers_mutex);
        } else if (choice == 5) {
            print_log("reader - main", "Послан сигнал на вывод прогноза продаж...");
            pthread_mutex_unlock(&trendline_mutex);
        } else if (choice == 6) {
            printf("\nВ отчет будут записаны следующие данные:\n");
            pthread_mutex_unlock(&sum_mutex);
            pthread_mutex_unlock(&top_mutex);

            sleep(1);

            print_log("reader - main", "Послан сигнал на вывод отчета...");
            pthread_mutex_unlock(&report_mutex);
        } else if (choice == 0) {
            print_log("reader - main", "Завершение работы...");
            working = 0;
            pthread_mutex_unlock(&read_mutex);
            pthread_mutex_unlock(&sum_mutex);
            pthread_mutex_unlock(&top_mutex);
            pthread_mutex_unlock(&customers_mutex);
            pthread_mutex_unlock(&trendline_mutex);
            pthread_mutex_unlock(&report_mutex);
        }
    }

    pthread_join(read_thread, NULL);
    pthread_join(sum_thread, NULL);
    pthread_join(top_thread, NULL);
    pthread_join(customer_thread, NULL);
    pthread_join(trend_thread, NULL);
    pthread_join(rep_thread, NULL);

    print_log("reader - main", "Потоки успешно завершены.");

    if (report_data.most_popular_items) free(report_data.most_popular_items);
    if (report_data.least_popular_items) free(report_data.least_popular_items);
    if (sales) free(sales);

    print_log("reader - main", "Работа завершена.");

    return 0;
}