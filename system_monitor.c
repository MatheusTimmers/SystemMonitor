#include "fetch_info.h"
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>

const char *fortunes[] = {
    "O pássaro madrugador pega a minhoca, mas o segundo rato pega o queijo.",
    "A programação hoje é uma corrida entre engenheiros de software tentando "
    "criar programas cada vez mais à prova de idiotas e o universo tentando "
    "criar idiotas cada vez maiores e melhores.",
    "Seu kernel acabou de ganhar na loteria das condições de corrida!",
    "Não é esperado que você entenda isso.",
    "Existem apenas dois tipos de linguagens de programação: aquelas das quais "
    "as pessoas sempre reclamam e aquelas que ninguém usa."};

void create_header(FILE *html_file){
  fprintf(html_file, "<!DOCTYPE html>\n");
  fprintf(html_file, "<html lang=\"pt-br\">\n<head>\n");
  fprintf(html_file, "<meta charset=\"UTF-8\">\n");
  fprintf(html_file, "<meta name=\"viewport\" content=\"width=device-width, "
                     "initial-scale=1.0\">\n");
  fprintf(html_file, "<title>Status do Sistema</title>\n");
  fprintf(html_file, "<style>\n");
  fprintf(html_file, "body { font-family: Arial, sans-serif; background-color: "
                     "#f4f4f9; color: #333; }\n");
  fprintf(html_file, "h1 { text-align: center; color: #444; }\n");
  fprintf(html_file, ".container { width: 80%%; margin: auto; padding: 20px; "
                     "background-color: #fff; border-radius: 10px; box-shadow: "
                     "0 0 10px rgba(0, 0, 0, 0.1); }\n");
  fprintf(html_file, "table { width: 100%%; border-collapse: collapse; "
                     "margin-bottom: 20px; }\n");
  fprintf(html_file, "table, th, td { border: 1px solid #ddd; }\n");
  fprintf(html_file, "th, td { padding: 10px; text-align: left; }\n");
  fprintf(html_file, "th { background-color: #f2f2f2; }\n");
  fprintf(html_file, "</style>\n</head>\n");
  fprintf(html_file, "<body>\n");
  fprintf(html_file, "<div class=\"container\">\n");
  fprintf(html_file, "<h1>Monitoramento do Sistema</h1>\n");
  fprintf(html_file, "<h2>Informações do Projeto</h2>\n");
  fprintf(html_file, "<p><strong>Trabalho 1 - Construção de Sistemas "
                     "Operacionais</strong></p>\n");
  fprintf(
      html_file,
      "<p>Integrantes: Matheus Timmers, Pedro de Ros e Matheus Robin</p>\n");
  fprintf(html_file, "<h2>Status do Sistema</h2>\n");
  fprintf(html_file, "<table>\n");
}

void update_html() {
  int index = rand() % (6);
  FILE *html_file;

  html_file = fopen("index.html", "w");

  if (!html_file)
    return;

  flock(fileno(html_file), LOCK_EX);

  create_header(html_file);

  // Coleta de informações
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

  // Exibe as informações coletadas em formato de tabela
  fprintf(html_file, "<tr><th>Versão do Sistema</th><td>%s</td></tr>\n",
          version);
  fprintf(html_file, "<tr><th>Uptime</th><td>%s</td></tr>\n", uptime);
  fprintf(html_file, "<tr><th>Tempo Ocioso</th><td>%s</td></tr>\n", idle_time);
  fprintf(html_file, "<tr><th>Data e Hora</th><td>%s</td></tr>\n", system_time);
  fprintf(html_file,
          "<tr><th>Informações do Processador</th><td>%s</td></tr>\n",
          cpu_info);
  fprintf(html_file, "<tr><th>Carga Média</th><td>%s</td></tr>\n",
          load_average);
  fprintf(html_file, "<tr><th>Uso de CPU</th><td>%s</td></tr>\n", cpu_usage);
  fprintf(html_file, "<tr><th>Informações de Memória</th><td>%s</td></tr>\n",
          memory_info);
  fprintf(html_file, "<tr><th>Estatísticas de I/O</th><td>%s</td></tr>\n",
          io_stats);
  fprintf(html_file,
          "<tr><th>Sistemas de Arquivos Suportados</th><td>%s</td></tr>\n",
          supported_filesystems);
  fprintf(html_file,
          "<tr><th>Dispositivos de Bloco e Caracteres</th><td>%s</td></tr>\n",
          devices);
  fprintf(html_file, "<tr><th>Dispositivos de Rede</th><td>%s</td></tr>\n",
          network);
  fprintf(html_file, "<tr><th>Processos Ativos</th><td>%s</td></tr>\n",
          processes);
  fprintf(html_file, "</table>\n");
  fprintf(html_file,
          "<footer>\n  <p>&copy; 2024 Monitoramento de Sistemas. Todos os "
          "direitos reservados.</p>\n <p>%s</p>\n</footer>\n",
          fortunes[index]);
  fprintf(html_file, "</div>\n");
  fprintf(html_file, "</body>\n</html>\n");

  // Liberação de memória
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

  // Destranca o arquivo e fecha
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
