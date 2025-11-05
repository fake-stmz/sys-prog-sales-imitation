#include "writer.h"

void* writer_thread(void* arg) {
    WriterArgs* args = (WriterArgs*)arg;
    Item* items = args->items;
    Customer* customers = args->customers;
    
    FILE* output_file = fopen(OUTPUT_FILE, "a");
    while (writing_active) {
        int item_index = rand() % 12;
        int customer_index = rand() % 4;

        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char date_time[20];
        strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", t);

        fprintf(output_file, "%d,%s,%d,%d,%d\n",
                next_sale_id++,
                date_time,
                customers[customer_index].id,
                items[item_index].id,
                (rand() % 5) + 1
                );
        
        fflush(output_file);

        int sleep_time = (rand() % 106) + 15;
        for (int i = 0; i < sleep_time && writing_active; i++) {
            if (!writing_active) {
                break;
            sleep(1);
        }
    }
    fclose(output_file);
    return NULL;
}