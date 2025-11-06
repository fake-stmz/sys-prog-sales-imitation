#include "reader.h"

void print_report_to_file() {
    FILE* report = fopen(REPORT_FILE, "w");
    fprintf(report, "Сумма продаж: %d\n", report_data.sales_sum);
    fprintf(report, "Прибыль: %d\n", report_data.profit);
    fprintf(report, "Наиболее популярные товары:\n");
    print_entries(report_data.most_popular_items, report);
    fprintf(report, "Наименее популярные товары:\n");
    print_entries(report_data.least_popular_items, report);
    fclose(report);
}

void* report_thread(void* arg) {
    
    pthread_mutex_lock(&read_mutex);

    while (working) {
        print_report_to_file();
        pthread_mutex_lock(&read_mutex);
    }

    return NULL;
}