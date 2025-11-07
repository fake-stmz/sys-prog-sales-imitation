#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <pthread.h>

#define LOG_FILE "./data/sales-imitation.log"

void print_log(char* source, char* format);

#endif