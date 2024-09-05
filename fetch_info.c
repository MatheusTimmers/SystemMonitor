#include "fetch_info.h"

FILE *get_file(char *name) {
  FILE *file;

  file = fopen(name, "r");
  if (file == NULL) {
    perror("Erro ao abrir /proc/uptime");
    return NULL;
  }

  return file;
}

// Busca a versão do sistema
char *fetch_version() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/version");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fgets(buffer, BUFFER_SIZE, file);
  fclose(file);

  return buffer;
}

// Busca uptime do sistema
char *fetch_uptime() {
  FILE *file;
  double uptime_seconds;
  char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/uptime");
  if (file == NULL) {
    free(result);
    return NULL;
  }

  fscanf(file, "%lf", &uptime_seconds);
  fclose(file);

  int days = (int)(uptime_seconds / 86400);
  int hours = (int)(uptime_seconds / 3600);
  int minutes = (int)(uptime_seconds / 60);
  int seconds = (int)(uptime_seconds);

  snprintf(result, BUFFER_SIZE, "Uptime: %d days, %02d:%02d:%02d", days,
           (hours % 24), (minutes % 60), (seconds % 60));
  return result;
}

// Busca o tempo ocioso do sistema
char *fetch_idle_time() {
  FILE *file;
  double idle_seconds;
  char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/uptime");
  if (file == NULL) {
    free(result);
    return NULL;
  }

  fscanf(file, "%*lf %lf", &idle_seconds); // Ignora o uptime
  fclose(file);

  int days = (int)(idle_seconds / 86400);
  int hours = (int)(idle_seconds / 3600);
  int minutes = (int)(idle_seconds / 60);
  int seconds = (int)(idle_seconds);

  snprintf(result, BUFFER_SIZE, "Idle Time: %d days, %02d:%02d:%02d", days,
           (hours % 24), (minutes % 60), (seconds % 60));
  return result;
}

char *fetch_system_time() {
  time_t now = time(NULL);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (buffer == NULL) {
    return NULL;
  }

  strftime(buffer, BUFFER_SIZE, "System Time: %Y-%m-%d %H:%M:%S",
           localtime(&now));
  return buffer;
}

char *fetch_cpu_info() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char line[BUFFER_SIZE];

  file = get_file("/proc/cpuinfo");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "model name", 10) == 0) {
      strcpy(buffer, strchr(line, ':') + 2); // Pega o valor após ": "
      break;
    }
  }
  fclose(file);

  return buffer;
}

char *fetch_load_average() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/loadavg");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fgets(buffer, BUFFER_SIZE, file);
  fclose(file);

  return buffer;
}

char *fetch_cpu_usage() {
  FILE *file;
  long user, nice, system, idle, total;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/stat");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fscanf(file, "cpu  %ld %ld %ld %ld", &user, &nice, &system, &idle);
  fclose(file);

  total = user + nice + system + idle;
  int cpu_usage = (int)(((total - idle) * 100) / total);

  snprintf(buffer, BUFFER_SIZE, "CPU Usage: %d%%", cpu_usage);
  return buffer;
}

char *fetch_memory_info() {
  FILE *file;
  long mem_total, mem_free;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  file = get_file("/proc/meminfo");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fscanf(file, "MemTotal: %ld kB\nMemFree: %ld kB", &mem_total, &mem_free);
  fclose(file);

  long mem_used = mem_total - mem_free;
  snprintf(buffer, BUFFER_SIZE, "Memory: Total = %ld MB, Used = %ld MB",
           mem_total / 1024, mem_used / 1024);

  return buffer;
}

char *fetch_io_stats() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  long read_bytes, write_bytes;

  file = get_file("/proc/self/io");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  while (fgets(buffer, BUFFER_SIZE, file)) {
    if (strncmp(buffer, "read_bytes", 10) == 0) {
      sscanf(buffer, "read_bytes: %ld", &read_bytes);
    } else if (strncmp(buffer, "write_bytes", 11) == 0) {
      sscanf(buffer, "write_bytes: %ld", &write_bytes);
    }
  }
  fclose(file);

  snprintf(buffer, BUFFER_SIZE,
           "I/O Stats: Read = %ld bytes, Write = %ld bytes", read_bytes,
           write_bytes);
  return buffer;
}

char *fetch_supported_filesystems() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char line[BUFFER_SIZE];

  file = get_file("/proc/filesystems");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  strcpy(buffer, "Supported Filesystems:\n");

  while (fgets(line, sizeof(line), file)) {
    strcat(buffer, line);
  }
  fclose(file);

  return buffer;
}
