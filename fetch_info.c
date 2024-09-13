#include "fetch_info.h"

FILE *get_file(char *name) {
  FILE *file;

  file = fopen(name, "r");
  if (file == NULL) {
    perror("Erro ao abrir arquivo");
    return NULL;
  }

  return file;
}

// Busca a versão do sistema
char *fetch_version() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (!buffer)
    return NULL;

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
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (!buffer)
    return NULL;

  file = get_file("/proc/uptime");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fscanf(file, "%lf", &uptime_seconds);
  fclose(file);

  int days = (int)(uptime_seconds / 86400);
  int hours = (int)(uptime_seconds / 3600);
  int minutes = (int)(uptime_seconds / 60);
  int seconds = (int)(uptime_seconds);

  snprintf(buffer, BUFFER_SIZE, "%d days, %02d:%02d:%02d", days, (hours % 24),
           (minutes % 60), (seconds % 60));
  return buffer;
}

// Busca o tempo ocioso do sistema
char *fetch_idle_time() {
  FILE *file;
  double idle_seconds;
  char *result = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (!result)
    return NULL;

  file = get_file("/proc/uptime");
  if (file == NULL) {
    free(result);
    return NULL;
  }

  fscanf(file, "%*f %lf", &idle_seconds); // Ignora o uptime
  fclose(file);

  int days = (int)(idle_seconds / 86400);
  int hours = (int)(idle_seconds / 3600);
  int minutes = (int)(idle_seconds / 60);
  int seconds = (int)(idle_seconds);

  snprintf(result, BUFFER_SIZE, "I%d days, %02d:%02d:%02d", days, (hours % 24),
           (minutes % 60), (seconds % 60));
  return result;
}

char *fetch_system_time() {
  time_t now = time(NULL);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (buffer == NULL) {
    return NULL;
  }

  strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&now));
  return buffer;
}

// Busca CPU info
char *fetch_cpu_info() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char line[LINE_SIZE];

  if (!buffer)
    return NULL;

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

// Busca a carga media
char *fetch_load_average() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  double load1, load5, load15;
  int running_processes, total_processes;
  int last_pid;

  file = get_file("/proc/loadavg");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fscanf(file, "%lf %lf %lf %d/%d %d", &load1, &load5, &load15, &running_processes, &total_processes, &last_pid);

  snprintf(buffer, BUFFER_SIZE,
           "Carga Média (1 min): %.2f - Carga Média (5 min): %.2f Carga Média (15 min): %2.f"
           "Processos em Execução: %d Total de Processos: %d Último PID %d <br>"
           , load1, load5, load15, running_processes, total_processes, last_pid);


  fclose(file);

  return buffer;
}

// Busca o uso do cpu
char *fetch_cpu_usage() {
  FILE *file;
  long user, nice, system, idle, total;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (!buffer)
    return NULL;

  file = get_file("/proc/stat");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fscanf(file, "cpu  %ld %ld %ld %ld", &user, &nice, &system, &idle);
  fclose(file);

  total = user + nice + system + idle;
  int cpu_usage = (int)(((total - idle) * 100) / total);

  snprintf(buffer, BUFFER_SIZE, "%d%%", cpu_usage);
  return buffer;
}

// Busca a informacao de memoria
char *fetch_memory_info() {
  FILE *file;
  long mem_total, mem_free;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (!buffer)
    return NULL;

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

// Busca o status do IO
char *fetch_io_stats() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  long read_bytes, write_bytes;

  if (!buffer)
    return NULL;

  file = get_file("/proc/diskstats");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  fgets(buffer, BUFFER_SIZE, file);

  fclose(file);

  return buffer;
}

// Busca sistemas de arquivos suportados
char *fetch_supported_filesystems() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char *buffer_pseudo = (char *)malloc(BUFFER_SIZE / 2 * sizeof(char));
  char *buffer_block = (char *)malloc(BUFFER_SIZE / 2 * sizeof(char));
  char line[LINE_SIZE];

  strcpy(buffer_pseudo, "<b>Sistemas de arquivos pseudo:</b>");
  strcpy(buffer_block, "<b>Sistemas de arquivos de dispositivo de bloco:</b>");

  file = get_file("/proc/filesystems");
  if (file == NULL) {
    free(buffer_pseudo);
    free(buffer_block);
    free(buffer);
    return NULL;
  }

  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "nodev") != NULL) {
        strncat(buffer_pseudo, " - ", BUFFER_SIZE / 2- strlen(buffer_pseudo) - 1);
        strncat(buffer_pseudo, line + 6, BUFFER_SIZE / 2 - strlen(buffer_pseudo) - 1);  // Pula "nodev" e o espaço seguinte
    } else {
        strncat(buffer_block, " - ", BUFFER_SIZE / 2 - strlen(buffer_block) - 1);
        strncat(buffer_block, line, BUFFER_SIZE / 2 - strlen(buffer_block) - 1);
    }
  }

  strncpy(buffer, buffer_block, BUFFER_SIZE - 1);
  buffer[BUFFER_SIZE - 1] = '\0';
  strncat(buffer, "<br>", BUFFER_SIZE - strlen(buffer) - 1);
  strncat(buffer, buffer_pseudo, BUFFER_SIZE - strlen(buffer) - 1);

  // Libera memória dos buffers intermediários
  free(buffer_pseudo);
  free(buffer_block);

  fclose(file);

  return buffer;
}

// Busca Dispositivos de Bloco e Caracteres
// Função horrivel de feia
char *fetch_block_char_devices() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char *buffer_char = (char *)malloc(BUFFER_SIZE / 2 * sizeof(char));
  char *buffer_block = (char *)malloc(BUFFER_SIZE / 2 * sizeof(char));
  char line[LINE_SIZE];

  strcpy(buffer, "");
  strcpy(buffer_char, "");
  strcpy(buffer_block, "");

  file = get_file("/proc/devices");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  // Pula a primeira linha
  fgets(line, sizeof(line), file);
  strncat(buffer_char, "<b>Character devices:</b><br>", BUFFER_SIZE / 2- strlen(buffer_char) - 1);
  
  // Espero que sempre seja primeiro caracteres e depois os blocks
  while (fgets(line, sizeof(line), file)) {
    // Chegamos na proxima lista
    if (strstr(line, "Block devices:")) {
      strncat(buffer_block, "<b>Block devices:</b><br>", BUFFER_SIZE / 2- strlen(buffer_char) - 1);
      break;
    }

    strncat(buffer_char, line, BUFFER_SIZE / 2- strlen(buffer_char) - 1);
    strncat(buffer_char, "<br>", BUFFER_SIZE / 2- strlen(buffer_char) - 1);
  }

  while (fgets(line, sizeof(line), file)) {
    strncat(buffer_block, line, BUFFER_SIZE / 2- strlen(buffer_block) - 1);
    strncat(buffer_block, "<br>", BUFFER_SIZE / 2- strlen(buffer_block) - 1);
  }

  // Junta os dois buffers
  strncpy(buffer, buffer_char, BUFFER_SIZE - 1);
  buffer[BUFFER_SIZE - 1] = '\0';
  strncat(buffer, "<br>", BUFFER_SIZE - strlen(buffer) - 1);
  strncat(buffer, buffer_block, BUFFER_SIZE - strlen(buffer) - 1);

  // Libera memória dos buffers intermediários
  free(buffer_char);
  free(buffer_block);

  fclose(file);
  return buffer;
}

// Busca a lista de dispositivos de rede
char *fetch_network_devices() {
  FILE *file;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char line[BUFFER_SIZE];

  strcpy(buffer, "");

  file = get_file("/proc/net/dev");
  if (file == NULL) {
    free(buffer);
    return NULL;
  }

  // Ignora as duas primeiras linhas de cabeçalhos
  fgets(line, sizeof(line), file);
  fgets(line, sizeof(line), file);

  // Adiciona cabeçalhos personalizados à saída formatada
  strcat(buffer, "<table border='1'>\n");
  strcat(buffer, "<tr><th>Interface</th><th>Recebido (bytes)</th><th>Transmitido (bytes)</th><th>Pacotes Recebidos</th><th>Pacotes Transmitidos</th></tr>\n");


  while (fgets(line, sizeof(line), file)) {
    char interface[LINE_SIZE];
    long rx_bytes, rx_pack, tx_bytes, tx_pack;

    // Extrai a interface, bytes recebidos e transmitidos da linha
    sscanf(line, "%s %ld %ld %*d %*d %*d %*d %*d %*d %ld %ld", interface, &rx_bytes, &rx_pack, &tx_bytes, &tx_pack);

    // Remove o ":" após o nome da interface
    char *colon_pos = strchr(interface, ':');
    if (colon_pos) {
      *colon_pos = '\0';
    }

    // Adiciona a linha da tabela com os dados extraídos
    char row[BUFFER_SIZE];
    snprintf(row, sizeof(row),
             "<tr><td>%s</td><td>%ld bytes</td><td>%ld bytes</td><td>%ld</td><td>%ld</td></tr>\n",
             interface, rx_bytes, tx_bytes, rx_pack, tx_pack);

    strcat(buffer, row);
  }

  strcat(buffer, "</table>");

  fclose(file);
  return buffer;
}

// Busca a lista de processos
char *fetch_process_list() {
  DIR *dir;
  struct dirent *entry;
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char proc_name[LINE_SIZE];;
  char path[LINE_SIZE];;
  FILE *file;

  if (!buffer)
    return NULL;

  dir = opendir("/proc");
  if (dir == NULL) {
    perror("Erro ao abrir /proc");
    free(buffer);
    return NULL;
  }

  // Percorre o proc para encontrar os PIDs
  while ((entry = readdir(dir)) != NULL) {
    // Verifica se é um PID
    if (isdigit(entry->d_name[0])) {
      snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

      file = fopen(path, "r");
      if (file != NULL) {
        fgets(proc_name, sizeof(proc_name), file);
        snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer),
                 "%s - %s", entry->d_name, proc_name);
        fclose(file);
      }
    }
  }

  closedir(dir);
  return buffer;
}
