#include "reader.h"

void read_sales() {
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

    sales = (Sale*)malloc(capacity * sizeof(Sale));
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
        
        Sale sale;
        if (sscanf(line, "%d,%19[^,],%d,%d,%d",
                   &sale.sale_id, sale.date_time, &sale.customer_id,
                   &sale.item_id, &sale.quantity) == 5) {
            
            if (count >= capacity) {
                capacity *= 2;
                Sale* temp = (Sale*)realloc(sales, capacity * sizeof(Sale));
                if (temp == NULL) {
                    perror("Failed to reallocate memory");
                    free(sales);
                    fclose(sales_file);
                    return;
                }
                sales = temp;
            }
            
            sales[count] = sale;
            count++;
        }
    }
    
    fclose(sales_file);
    
    if (count > 0) {
        Sale* temp = (Sale*)realloc(sales, count * sizeof(Sale));
        if (temp != NULL) {
            sales = temp;
        }
    }
    printf("wow\n");
}

void* read_sales_thread(void* arg) {

    while (working) {
        read_sales();
        sem_wait(&read_sem);
    }

    return NULL;
}