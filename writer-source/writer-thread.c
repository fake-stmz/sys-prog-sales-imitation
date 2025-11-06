#include "writer.h"

void* writer_thread(void* arg) {
    writer_args_t* args = (writer_args_t*)arg;
    item_t* items = args->items;
    customer_t* customers = args->customers;
    
    FILE* output_file = fopen(SALES_FILE, "a");
    while (writing_active) {
        pthread_mutex_lock(&sale_mutex);

        int item_index = rand() % 12;
        int customer_index = rand() % 4;

        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char date_time[20];
        strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", t);

        char output_line[256];

        int sale_quantity = (rand() % 5) + 1;

        sprintf(output_line, "%d,%s,%d,%d,%d",
            next_sale_id++,
            date_time,
            customers[customer_index].id,
            items[item_index].id,
            sale_quantity
        );
        
        printf("Записана продажа: %s купил %s в количестве %d (%s)\n", customers[customer_index].name, items[item_index].name, sale_quantity, output_line);
        fprintf(output_file, "%s\n", output_line);
        
        fflush(output_file);

        pthread_mutex_unlock(&sale_mutex);

        int sleep_time = (rand() % 106) + 15;
        for (int i = 0; i < sleep_time && writing_active; i++) {
            if (!writing_active) {
                break;
            }
            sleep(1);
        }
    }
    fclose(output_file);
    return NULL;
}