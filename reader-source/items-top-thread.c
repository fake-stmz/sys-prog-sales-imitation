#include "reader.h"

top_entry_t entries[12];

void init_entries() {
    for (int i = 0; i < 12; i++) {
        entries[i].item = items[i];
        entries[i].count = 0;
    }
}

void print_entries(top_entry_t* entries) {
    for (int i = 0; i < 5; i++) {
        printf("%d. %s - %d\n", i + 1, entries[i].item.name, entries[i].count);
    }
}

void calculate_top() {

    init_entries();

    pthread_rwlock_rdlock(&sales_rwlock);

    for (int i = 0; i < report_data.sales_count; i++) {
        entries[sales[i].item_id - 1].count += sales[i].quantity;
    }
    
    pthread_rwlock_unlock(&sales_rwlock);

    top_entry_t most_popular_items[5];
    top_entry_t least_popular_items[5];

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

    for (int i = 0; i < 5; i++) {
        most_popular_items[i] = entries[i];
        least_popular_items[i] = entries[11 - i];
    }

    report_data.most_popular_items = most_popular_items;
    report_data.least_popular_items = least_popular_items;

    printf("Самые популярные товары:\n");
    print_entries(most_popular_items);

    printf("Самые непопулярные товары:\n");
    print_entries(least_popular_items);
}

void* items_top_thread(void* arg) {
    
    while (working) {
        calculate_top();
        pthread_mutex_lock(&top_mutex);
    }

    return NULL;
}