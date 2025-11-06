#include "reader.h"

sale_t* sales = NULL;
volatile int working = 1;
pthread_rwlock_t sales_rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t read_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t top_mutex = PTHREAD_MUTEX_INITIALIZER;
report_data_t report_data;
item_t items[12];
customer_t customers[4];

void init_report_data(report_data_t* report_data) {
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
    return 0;
}