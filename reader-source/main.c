#include "reader.h"

sale_t* sales = NULL;
volatile int working = 1;
pthread_rwlock_t sales_rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t read_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {}