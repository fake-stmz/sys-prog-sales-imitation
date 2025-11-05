#include "reader.h"

Sale* sales = NULL;
int working = 1;
sem_t read_sem;

int main() {
    /* debug
    sem_init(&read_sem, 0, 0);

    pthread_t read_sales_thr;
    pthread_create(&read_sales_thr, NULL, read_sales_thread, NULL);

    sleep(5);
    printf("%d\n", sales[0].sale_id);
    sleep(3);

    sem_post(&read_sem);
    sleep(3);

    working = 0;
    sem_post(&read_sem);

    pthread_join(read_sales_thr, NULL);

    sem_destroy(&read_sem);
    return 0;
    */
}