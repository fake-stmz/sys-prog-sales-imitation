#include "reader.h"

float calculate_dot(int t_sum, int y_sum, int ty_sum, int t2_sum, int next_t) {
    print_log("reader - trendline_thread", "Высчитывание точки...");
    float a = (float) (ty_sum - (t_sum * y_sum)) / (float) (t2_sum - (t_sum * t_sum));
    float b = (float) y_sum - a * (float) t_sum;
    return a * (float) next_t + b;
}

void calculate_trendline() {
    int t_sum = 0, y_sum = 0, ty_sum = 0, t2_sum = 0;

    pthread_rwlock_rdlock(&sales_rwlock);

    print_log("reader - trendline_thread", "Высчитывание сумм...");
    for (int i = 0; i < report_data.sales_count; i++) {
        t_sum += i + 1;
        y_sum = sales[i].quantity;
        ty_sum += (i + 1) * sales[i].quantity;
        t2_sum += (i + 1) * (i + 1);
    }

    pthread_rwlock_unlock(&sales_rwlock);

    float three_next_sales[3];

    printf("Прогноз 3-х следующих продаж:\n");
    for (int i = 0; i < 3; i++) {
        three_next_sales[i] = calculate_dot(t_sum, y_sum, ty_sum, t2_sum, i + report_data.sales_count + i + 1);
        print_log("reader - trendline_thread", "Вывод прогноза...");
        printf("%d. %f товаров\n", i + 1, three_next_sales[i]);
    }
}

void* trendline_thread(void* arg) {
    print_log("reader - trendline_thread", "Начало работы.");

    pthread_mutex_lock(&trendline_mutex);
    
    while (working) {
        print_log("reader - trendline_thread", "Вычисление тенденций...");
        calculate_trendline();
        pthread_mutex_lock(&trendline_mutex);
    }

    print_log("reader - trendline_thread", "Завершение работы.");
    return NULL;
}