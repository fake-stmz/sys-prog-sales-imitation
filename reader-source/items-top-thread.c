#include "reader.h"

top_entry_t entries[12];

void init_entries() {
    print_log("reader - items_top_thread", "Инициализация данных о товарах...");
    for (int i = 0; i < 12; i++) {
        entries[i].item = items[i];
        entries[i].count = 0;
    }
    print_log("reader - items_top_thread", "Данные о товарах успешно инициализированы.");
}

void print_entries(top_entry_t* entries, FILE* to) {
    for (int i = 0; i < 5; i++) {
        fprintf(to, "%d. %s - %d\n", i + 1, entries[i].item.name, entries[i].count);
    }
}

void calculate_top() {
    init_entries();

    pthread_rwlock_rdlock(&sales_rwlock);

    print_log("reader - items_top_thread", "Получение данных о популярности товаров...");
    for (int i = 0; i < report_data.sales_count; i++) {
        entries[sales[i].item_id - 1].count += sales[i].quantity;
    }
    print_log("reader - items_top_thread", "Данные о популярности товаров успешно получены.");
    
    pthread_rwlock_unlock(&sales_rwlock);

    print_log("reader - items_top_thread", "Сортировка данных о популярности товаров...");
    for (int i = 0; i < 5; i++) {
        int max_index = i;
        for (int j = i + 1; j < 12; j++) {
            if (entries[j].count > entries[max_index].count) {
                max_index = j;
            }
        }
        top_entry_t temp = entries[max_index];
        entries[max_index] = entries[i];
        entries[i] = temp;
    }
    print_log("reader - items_top_thread", "Данные о популярности товаров успешно отсортированы.");

    print_log("reader - items_top_thread", "Копирование данных о популярности товаров...");
    for (int i = 0; i < 5; i++) {
        if (report_data.most_popular_items) {
            report_data.most_popular_items[i] = entries[i];
        }
        if (report_data.least_popular_items) {
            report_data.least_popular_items[i] = entries[11 - i];
        }
    }
    print_log("reader - items_top_thread", "Данные о популярности товаров успешно скопированы.");

    sleep(1);

    print_log("reader - items_top_thread", "Вывод данных о популярности товаров...");
    printf("Самые популярные товары:\n");
    print_entries(report_data.most_popular_items, stdout);

    printf("Самые непопулярные товары:\n");
    print_entries(report_data.least_popular_items, stdout);
}

void* items_top_thread(void* arg) {
    print_log("reader - items_top_thread", "Начало работы.");

    pthread_mutex_lock(&top_mutex);

    while (working) {
        print_log("reader - items_top_thread", "Получение данных о популярности товаров...");
        calculate_top();
        pthread_mutex_lock(&top_mutex);
    }

    print_log("reader - items_top_thread", "Завершение работы.");
    return NULL;
}