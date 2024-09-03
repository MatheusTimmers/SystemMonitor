#include <stdio.h>
#include <sys/file.h>

void update_html() {
  char buffer[128];
  FILE *rtc_file;
  FILE *html_file;

  // Busca o arquivo RTC, com as informacaoes RealTime
  rtc_file = fopen("/proc/driver/rtc", "r");
  if (rtc_file == NULL) {
    perror("Erro ao abrir /proc/driver/rtc");
    return;
  }

  // Busca o arquivo HTML
  html_file = fopen("index.html", "w");
  if (html_file == NULL) {
    perror("Erro ao abrir index.html");
    fclose(rtc_file);
    return;
  }

  flock(fileno(html_file), LOCK_EX);

  // Limpar o arquivo HTML e escrever o cabeçalho básico
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

  // Ler e escrever as informações do /proc/driver/rtc no HTML
  while (fgets(buffer, sizeof(buffer), rtc_file)) {
    fprintf(html_file, "<p>%s</p>\n", buffer);
  }

  // Fechar as tags HTML
  fprintf(html_file, "</body></html>\n");

  // Liberar a trava e fechar os arquivos
  flock(fileno(html_file), LOCK_UN);
  fclose(html_file);
  fclose(rtc_file);
}

int main() {
  while (1) {
    update_html();
    sleep(3000);
  }
  return 0;
}
