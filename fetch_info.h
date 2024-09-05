#ifndef FETCH_INFO_H
#define FETCH_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>

#define ERROR_GET_FILE "Erro ao abrir o arquivo"
#define BUFFER_SIZE 4096

FILE *get_file(char *name);

char *fetch_version();
char *fetch_uptime();
char *fetch_idle_time();
char *fetch_system_time();
char *fetch_cpu_info();
char *fetch_load_average();
char *fetch_cpu_usage();
char *fetch_memory_info();
char *fetch_io_stats();
char *fetch_supported_filesystems();

#endif
