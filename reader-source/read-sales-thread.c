#include "reader.h"

void read_sales() {
    pthread_rwlock_wrlock(&sales_rwlock);
    free(sales);
    sales = NULL;

    FILE* sales_file = fopen(SALES_FILE, "r");
    if (!sales_file) {
        perror("Read sales thread - Sales file");
        return;
    }
    
    char line[256];
    int capacity = 10;
    int count = 0;

    sales = (sale_t*)malloc(capacity * sizeof(sale_t));
    if (sales == NULL) {
        perror("Read sales thread - init malloc");
        fclose(sales_file);
        return;
    }

    if (fgets(line, sizeof(line), sales_file) == NULL) {
        fclose(sales_file);
        return;
    }

    while (fgets(line, sizeof(line), sales_file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        sale_t sale;
        sscanf(line, "%d,%[^,],%d,%d,%d", &sale.sale_id, sale.date_time, &sale.customer_id, &sale.item_id, &sale.quantity);
        sales[count] = sale;
        count++;
        if (count == capacity) {
            capacity *= 2;
            sale_t* temp = (sale_t*)realloc(sales, capacity * sizeof(sale_t));
            if (temp != NULL) {
                sales = temp;
            }
        }
    }
    
    fclose(sales_file);
    
    if (count > 0) {
        sale_t* temp = (sale_t*)realloc(sales, count * sizeof(sale_t));
        if (temp != NULL) {
            sales = temp;
        }
        report_data.sales_count = count;
    }
    pthread_rwlock_unlock(&sales_rwlock);
}

void* read_sales_thread(void* arg) {

    while (working) {
        read_sales();
        pthread_mutex_lock(&read_mutex);
    }

    return NULL;
}