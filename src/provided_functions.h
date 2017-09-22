#include <sys/stat.h>

int trocear_cadena(char *cadena, char *trozos[]);
char tipo_fichero(mode_t m);
char *convierte_modo(mode_t m, char *permisos);
char *convierte_modo2(mode_t m);
char *convierte_modo3(mode_t m);
