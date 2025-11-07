#include "reader.h"

void print_report_to_file() {
    char current_date[20];
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(current_date, sizeof(current_date), "%Y-%m-%d %H:%M:%S", timeinfo);
    report_data.date = current_date;

    FILE* report = fopen(REPORT_FILE, "w");
    fprintf(report, "=== Отчет по продажам (дата: %s) ===\n\n", report_data.date);
    fprintf(report, "Количество продаж: %d\n\n", report_data.sales_count);
    fprintf(report, "Сумма продаж: %d\n\n", report_data.sales_sum);
    fprintf(report, "Прибыль: %d\n\n", report_data.profit);
    fprintf(report, "Наиболее популярные товары:\n");
    print_entries(report_data.most_popular_items, report);
    fprintf(report, "\n");
    fprintf(report, "Наименее популярные товары:\n");
    print_entries(report_data.least_popular_items, report);
    fprintf(report, "\n=== Конец отчета ===\n");
    fclose(report);

    print_log("reader - report_thread", "Отчет успешно создан.");
}

void* report_thread(void* arg) {
    print_log("reader - report_thread", "Начало работы.");
    
    pthread_mutex_lock(&report_mutex);

    while (working) {
        print_log("reader - report_thread", "Создание отчета...");
        print_report_to_file();
        pthread_mutex_lock(&report_mutex);
    }

    print_log("reader - report_thread", "Завершение работы.");
    return NULL;
}