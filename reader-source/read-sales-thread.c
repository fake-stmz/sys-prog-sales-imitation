#include "reader.h"

void read_sales() {
    pthread_rwlock_wrlock(&sales_rwlock);
    free(sales);
    sales = NULL;

    print_log("reader - read_sales_thread", "Открытие файла продаж...");
    FILE* sales_file = fopen(SALES_FILE, "r");
    if (!sales_file) {
        perror("Read sales thread - Sales file");
        return;
    }
    print_log("reader - read_sales_thread", "Файл продаж успешно открыт.");
    
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

    print_log("reader - read_sales_thread", "Чтение продаж...");
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
    print_log("reader - read_sales_thread", "Продажи успешно прочитаны.");
    
    fclose(sales_file);
    
    print_log("reader - read_sales_thread", "Выделение памяти для продаж...");
    if (count > 0) {
        sale_t* temp = (sale_t*)realloc(sales, count * sizeof(sale_t));
        if (temp != NULL) {
            sales = temp;
        }
        report_data.sales_count = count;
    }
    print_log("reader - read_sales_thread", "Память для продаж успешно выделена.");
    pthread_rwlock_unlock(&sales_rwlock);
}

void* read_sales_thread(void* arg) {
    print_log("reader - read_sales_thread", "Начало работы.");

    while (working) {
        print_log("reader - read_sales_thread", "Чтение данных о продажах...");
        read_sales();
        pthread_mutex_lock(&read_mutex);
    }

    print_log("reader - read_sales_thread", "Завершение работы.");
    return NULL;
}