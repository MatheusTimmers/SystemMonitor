#include "fetch_info.h"
#include <stdio.h>
#include <unistd.h>

void update_html() {
  FILE *html_file;

  html_file = fopen("index.html", "w");

  if (!html_file)
    return;

  flock(fileno(html_file), LOCK_EX);

  fprintf(html_file, "<html>\n");
  fprintf(html_file, "</head>\n");
  fprintf(html_file, "<body>\n");
  fprintf(html_file, "    <div class=\"container\">\n");
  fprintf(html_file, "        <h1>Construção de Sistemas Operacionais</h1>\n");
  fprintf(html_file, "        <div class=\"left-align\">\n");
  fprintf(html_file, "            <p><strong>Trabalho 1</strong></p>\n");
  fprintf(html_file, "            <p>Integrantes: Matheus Timmers, Pedro de "
                     "Ros e Matheus Robin</p>\n");
  fprintf(html_file, "        </div>\n");
  fprintf(html_file, "    </div>\n");

  char *idle_time = fetch_idle_time();
  char *uptime = fetch_uptime();
  char *version = fetch_version();
  char *load_average = fetch_load_average();
  char *cpu_info = fetch_cpu_info();
  char *system_time = fetch_system_time();
  char *memory_info = fetch_memory_info();
  char *cpu_usage = fetch_cpu_usage();
  char *io_stats = fetch_io_stats();
  char *supported_filesystems = fetch_supported_filesystems();
  char *devices = fetch_block_char_devices();
  char *network = fetch_network_devices();
  char *processes = fetch_process_list();

  fprintf(html_file, "<p>%s</p>\n", idle_time);
  fprintf(html_file, "<p>%s</p>\n", uptime);
  fprintf(html_file, "<p>%s</p>\n", version);
  fprintf(html_file, "<p>%s</p>\n", load_average);
  fprintf(html_file, "<p>%s</p>\n", cpu_info);
  fprintf(html_file, "<p>%s</p>\n", system_time);
  fprintf(html_file, "<p>%s</p>\n", memory_info);
  fprintf(html_file, "<p>%s</p>\n", cpu_usage);
  fprintf(html_file, "<p>%s</p>\n", io_stats);
  fprintf(html_file, "<p>%s</p>\n", supported_filesystems);
  fprintf(html_file, "<p>%s</p>\n", devices);
  fprintf(html_file, "<p>%s</p>\n", network);
  fprintf(html_file, "<p>%s</p>\n", processes);

  // Libere a memória após o uso
  free(idle_time);
  free(uptime);
  free(version);
  free(load_average);
  free(cpu_info);
  free(system_time);
  free(memory_info);
  free(cpu_usage);
  free(io_stats);
  free(supported_filesystems);
  free(devices);
  free(network);
  free(processes);

  fprintf(html_file, "</body></html>\n");

  flock(fileno(html_file), LOCK_UN);
  fclose(html_file);
}

int main() {
  while (1) {
    update_html();
    sleep(3);
  }
  return 0;
}
