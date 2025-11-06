#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void read_items(item_t* items) {
    FILE* items_file = fopen(ITEMS_FILE, "r");
    if (!items_file) {
        perror("Item file");
        exit(1);
    }

    char line[256];
    int index = 0;
    
    fgets(line, sizeof(line), items_file);

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
}

void read_customers(customer_t* customers) {
    FILE* customers_file = fopen(CUSTOMERS_FILE, "r");
    if (!customers_file) {
        perror("Customer file");
        exit(1);
    }

    char line[256];
    int index = 0;
    
    fgets(line, sizeof(line), customers_file);

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
