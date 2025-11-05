#include "writer.h"

#define ITEMS_COUNT 12
#define CUSTOMERS_COUNT 4
#define THREAD_COUNT 3

int writing_active = 0;
int next_sale_id = 1;
pthread_mutex_t sale_mutex = PTHREAD_MUTEX_INITIALIZER;

void read_data(Item* items, Customer* customers) {
    FILE* items_file = fopen(ITEMS_FILE, "r");
    FILE* customers_file = fopen(CUSTOMERS_FILE, "r");
    if (!items_file || !customers_file) {
        perror("Item or Customer file");
        exit(1);
    }

    char line[256];
    int index = 0;

    fgets(line, sizeof(line), items_file);
    fgets(line, sizeof(line), customers_file);

    while (fgets(line, sizeof(line), items_file) && index < 12) {
        char* token = strtok(line, ",");
        items[index].id = atoi(token);
        token = strtok(NULL, ",");
        items[index].name = strdup(token);
        token = strtok(NULL, ",");
        items[index].price = atoi(token);
        index++;
    }
    fclose(items_file);

    index = 0;
    while (fgets(line, sizeof(line), customers_file) && index < 4) {
        char* token = strtok(line, ",");
        customers[index].id = atoi(token);
        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = 0;
        customers[index].name = strdup(token);
        index++;
    }
    fclose(customers_file);
}

int main() {
    Item items[ITEMS_COUNT];
    Customer customers[CUSTOMERS_COUNT];
    WriterArgs args = {items, customers};
    read_data(items, customers);

    printf("Начинается запись продаж...\n");
    printf("Нажмите Enter для остановки записи.\n");

    writing_active = 1;

    FILE* sales_file = fopen(OUTPUT_FILE, "r");
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