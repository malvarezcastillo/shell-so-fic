#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <dirent.h>

#include "provided_functions.h"

#define ELEMENTOS_MAX 1024

int terminado = 0;
int es_recursivo = 0;

/*P0*/
void salir()
{
  terminado = 1;
}

void imprimir_autores(int numero_trozos, char *comando_troceado[])
{
  int i, nombres = 0, login = 0;
  for (i = 1; i < numero_trozos; i++)
  {
    if (strcmp("-l", comando_troceado[i]) == 0)
    {
      login = 1;
    }
    else if (strcmp("-n", comando_troceado[i]) == 0)
    {
      nombres = 1;
    }
    else if (strcmp("-h", comando_troceado[i]) == 0)
    {
      printf("Imprime los autores de este shell\n");
      printf("Uso: autores [-l|-n]\n");
      return;
    }
    else
    {
      printf("Parametro %s desconocido\n", comando_troceado[i]);
      return;
    }
  }

  if (!nombres && !login)
  {
    nombres = 1;
    login = 1;
  }

  if (nombres)
  {
    printf("Martin Alvarez Castillo");
  }

  if (nombres && login)
  {
    printf(" - ");
  }

  if (login)
  {
    printf("m.castillo@udc.es");
  }

  printf("\n");
}

void imprimir_pid(int numero_trozos, char *comando_troceado[])
{
  if (numero_trozos == 1)
  {
    printf("%d\n", getpid());
  }
  else if (numero_trozos > 1 && strcmp("-p", comando_troceado[1]) == 0)
  {

    printf("%d\n", getppid());
  }
  else if (numero_trozos > 1 && strcmp("-h", comando_troceado[1]) == 0)
  {
    printf("Imprime el PID del shell y de su proceso padre\n");
    printf("Uso: pid [-p]\n");
    return;
  }
  else
  {
    printf("Parametro %s desconocido\n", comando_troceado[1]);
    return;
  }
}

/*P1*/

void imprimir_info(const char *nombre)
{
  char fecha[256];
  struct stat atributos;
  struct passwd *pwd;
  struct group *grp;

  int salida = lstat(nombre, &atributos);
  if (salida == 0)
  {
    printf("%ld ", (long)atributos.st_ino);
    printf("%s", convierte_modo2(atributos.st_mode));
    printf("%ld ", (long)atributos.st_nlink);
    if ((pwd = getpwuid(atributos.st_uid)) != NULL)
    {
      printf("%-8.8s ", pwd->pw_name);
    }
    else
    {
      printf("%-8d ", atributos.st_uid);
    }
    if ((grp = getgrgid(atributos.st_gid)) != NULL)
    {
      printf("%-8.8s ", grp->gr_name);
    }
    else
    {
      printf("%-8d ", atributos.st_gid);
    }
    printf("%lld ", (long long)atributos.st_size);
    strftime(fecha, sizeof(fecha), "%b %e %H:%M",
             localtime(&(atributos.st_mtime)));
    printf("%s ", fecha);

    char symlink_s[PATH_MAX + 1];
    char *resultado_rp = realpath(nombre, symlink_s);
    int es_link = S_ISLNK(atributos.st_mode);

    if (es_link && resultado_rp)
    {
      printf("%s -> %s\n", nombre, symlink_s);
    }
    else
    {
      printf("%s\n", nombre);
    }
  }
  else
  {
    if (errno == 2)
    {
      printf("Fichero o directorio desconocido: %s\n", nombre);
    }
    else
    {
      printf("Error obteniendo info de fichero %s: %d %s\n",
             nombre, errno, strerror(errno));
    }
  }
}

void info(int numero_trozos, char *comando_troceado[])
{
  int i;

  for (i = 1; i < numero_trozos; i++)
  {
    imprimir_info(comando_troceado[i]);
  }
}

int es_directorio(const char *ruta)
{
  struct stat stat_ruta;
  stat(ruta, &stat_ruta);
  return S_ISDIR(stat_ruta.st_mode);
}

void listar_directorio(const char *nombre, int descripcion_larga)
{
  DIR *dir;
  struct dirent *entrada;

  if (!(dir = opendir(nombre)))
  {
    if (errno == 2)
    {
      printf("Directorio inexistente insuficientes privilegios: %s\n", nombre);
    }
    else
    {
      printf("Error obteniendo info de directorio %s: %d %s\n",
             nombre, errno, strerror(errno));
    }
    return;
  }

  printf("%s:\n", nombre);

  while ((entrada = readdir(dir)) != NULL)
  {
    if (strncmp(entrada->d_name, ".", 1) == 0)
    {
      continue;
    }
    if (descripcion_larga)
    {
      char ruta[PATH_MAX + 1];
      snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entrada->d_name);
      imprimir_info(ruta);
    }
    else
    {
      printf("%s ", entrada->d_name);
    }
  }
  printf("\n\n");
  closedir(dir);
  if (es_recursivo)
  {
    dir = opendir(nombre);

    while ((entrada = readdir(dir)) != NULL)
    {
      if (strncmp(entrada->d_name, ".", 1) == 0)
      {
        continue;
      }
      char ruta[PATH_MAX + 1];
      snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entrada->d_name);
      if (es_directorio(ruta))
      {
        listar_directorio(ruta, descripcion_larga);
      }
    }
    closedir(dir);
  }
}

void list(int numero_trozos, char *comando_troceado[])
{
  int i;
  int largo = 0;
  if (numero_trozos == 1)
  {
    listar_directorio(".", 0);
  }
  else if (numero_trozos == 2 && strcmp(comando_troceado[1], "-l") == 0)
  {
    listar_directorio(".", 1);
  }
  else
  {
    if (strcmp(comando_troceado[1], "-l") == 0)
    {
      largo = 1;
    }
    for (i = 1 + largo; i < numero_trozos; i++)
    {
      listar_directorio(comando_troceado[i], largo);
    }
  }
}

void recursive(int numero_trozos, char *comando_troceado[])
{
  if (numero_trozos >= 2)
  {
    if ((strcmp("ON", comando_troceado[1]) == 0) ||
        strcmp("OFF", comando_troceado[1]) == 0)
    {
      if ((strcmp("ON", comando_troceado[1]) == 0))
      {
        es_recursivo = 1;
      }
      else
      {
        es_recursivo = 0;
      }
    }
    else
    {
      printf("Parametro %s desconocido\n", comando_troceado[1]);
      printf("Uso: recursive [ON|OFF]\n");
      return;
    }
  }

  printf("Flag Recursive esta ");
  if (es_recursivo)
  {
    printf("ON\n");
  }
  else
  {
    printf("OFF\n");
  }
}

void eliminar_fichero(char *ruta)
{
  int salida;
  salida = unlink(ruta);
  if (salida == 0)
  {
    printf("Borrado: %s \n", ruta);
  }
  else
  {
    if (errno == 2)
    {
      printf("Fichero desconocido: %s\n", ruta);
    }
    else
    {
      printf("Error borrando fichero %s: %d %s\n",
             ruta, errno, strerror(errno));
    }
  }
}

void eliminar_recursivo(const char *nombre)
{
  DIR *dir;
  struct dirent *entrada;

  if (!(dir = opendir(nombre)))
  {
    if (errno == 2)
    {
      printf("Directorio inexistente insuficientes privilegios: %s\n", nombre);
    }
    else
    {
      printf("Error obteniendo info de directorio %s: %d %s\n",
             nombre, errno, strerror(errno));
    }
    return;
  }

  dir = opendir(nombre);

  while ((entrada = readdir(dir)) != NULL)
  {
    if (strcmp(".", entrada->d_name) == 0 || strcmp("..", entrada->d_name) == 0)
    {
      continue;
    }
    char ruta[PATH_MAX + 1];
    snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entrada->d_name);
    if (es_directorio(ruta))
    {
      eliminar_recursivo(ruta);
    }
    else
    {
      eliminar_fichero(ruta);
    }
  }
  int salida;
  salida = rmdir(nombre);
  if (salida == 0)
  {
    printf("Borrado: %s \n", nombre);
  }
  else
  {
    printf("Error borrando directorio %s: %d %s\n",
           nombre, errno, strerror(errno));
  }
  closedir(dir);
}

void eliminar_directorio(char *ruta, int forzar)
{
  int salida;
  salida = rmdir(ruta);
  if (salida == 0)
  {
    printf("Borrado: %s \n", ruta);
  }
  else
  {
    if (errno == 39)
    {
      //Directorio no vacío
      if (forzar)
      {
        eliminar_recursivo(ruta);
      }
      else
      {
        printf("No se pudo eliminar %s: Directorio no vacío\n", ruta);
      }
    }
    else if (errno == 2)
    {
      printf("Fichero desconocido: %s\n", ruta);
    }
    else
    {
      printf("Error borrando directorio %s: %d %s\n",
             ruta, errno, strerror(errno));
    }
  }
}

void eliminate(int numero_trozos, char *comando_troceado[])
{
  if (numero_trozos == 1)
  {
    return;
  }
  else if (numero_trozos == 2)
  {
    if (es_directorio(comando_troceado[1]))
    {
      eliminar_directorio(comando_troceado[1], 0);
    }
    else
    {
      eliminar_fichero(comando_troceado[1]);
    }
  }
  else if (numero_trozos == 3 && strcmp("-f", comando_troceado[1]) == 0)
  {
    if (es_directorio(comando_troceado[2]))
    {
      eliminar_directorio(comando_troceado[2], 1);
    }
    else
    {
      eliminar_fichero(comando_troceado[2]);
    }
  }
  else
  {
    printf("Parametro %s desconocido\n", comando_troceado[1]);
    printf("Uso: eliminate [-f] nombrefichero\n");
    return;
  }
}

void imprimir_prompt()
{
  printf("$ ");
}

char *leer_entrada()
{
  char *entrada_teclado = (char *)malloc(sizeof(char) * ELEMENTOS_MAX);
  fgets(entrada_teclado, ELEMENTOS_MAX, stdin);
  return entrada_teclado;
}

void procesar_entrada(char *entrada)
{
  char *comando_troceado[ELEMENTOS_MAX];
  int numero_trozos = trocear_cadena(entrada, comando_troceado);

  if (numero_trozos > 0)
  {
    if (strcmp("exit", comando_troceado[0]) == 0 ||
        strcmp("end", comando_troceado[0]) == 0 ||
        strcmp("fin", comando_troceado[0]) == 0)
    {
      salir();
    }
    else if (strcmp("autores", comando_troceado[0]) == 0)
    {
      imprimir_autores(numero_trozos, comando_troceado);
    }
    else if (strcmp("pid", comando_troceado[0]) == 0)
    {
      imprimir_pid(numero_trozos, comando_troceado);
    }
    else if (strcmp("info", comando_troceado[0]) == 0)
    {
      info(numero_trozos, comando_troceado);
    }
    else if (strcmp("list", comando_troceado[0]) == 0)
    {
      list(numero_trozos, comando_troceado);
    }
    else if (strcmp("recursive", comando_troceado[0]) == 0)
    {
      recursive(numero_trozos, comando_troceado);
    }
    else if (strcmp("eliminate", comando_troceado[0]) == 0)
    {
      eliminate(numero_trozos, comando_troceado);
    }
    else
    {
      printf("Comando desconocido: %s\n", comando_troceado[0]);
    }
  }
  free(entrada);
}

int main(void)
{
  while (!terminado)
  {
    imprimir_prompt();
    procesar_entrada(leer_entrada());
  }

  exit(0);
}