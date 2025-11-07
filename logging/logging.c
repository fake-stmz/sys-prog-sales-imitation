#include "logging.h"

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void print_log(char* source, char* format) {
    pthread_mutex_lock(&log_mutex);
    FILE* log_file = fopen(LOG_FILE, "a");
    fprintf(log_file, "[%s] %s\n", source, format);
    fclose(log_file);
    pthread_mutex_unlock(&log_mutex);
}