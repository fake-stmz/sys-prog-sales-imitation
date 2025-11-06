#include "writer.h"

#define ITEMS_COUNT 12
#define CUSTOMERS_COUNT 4
#define THREAD_COUNT 3

volatile int writing_active = 0;
volatile int next_sale_id = 1;
pthread_mutex_t sale_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    item_t items[ITEMS_COUNT];
    customer_t customers[CUSTOMERS_COUNT];
    writer_args_t args = {items, customers};
    read_items(items);
    read_customers(customers);

    writing_active = 1;

    FILE* sales_file = fopen(SALES_FILE, "r");
    if (sales_file) {
        char line[256];
        fgets(line, sizeof(line), sales_file);
        while (fgets(line, sizeof(line), sales_file)) {
            int sale_id;
            sscanf(line, "%d,", &sale_id);
            if (sale_id >= next_sale_id) {
                next_sale_id = sale_id + 1;
            }
        }
        fclose(sales_file);
    } else {
        next_sale_id = 1;
    }

    pthread_mutex_init(&sale_mutex, NULL);

    srand(time(NULL));

    printf("Начинается запись продаж...\n");
    printf("Нажмите Enter для остановки записи.\n");

    pthread_t threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, writer_thread, &args) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    getchar();
    writing_active = 0;

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&sale_mutex);
    printf("Запись продаж остановлена.\n");

    return 0;
}