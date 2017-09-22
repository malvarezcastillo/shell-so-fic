#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define ELEMENTOS_MAX 1024

int terminado = 0;

int trocear_cadena(char *cadena, char *trozos[])
{
  int i = 1;
  if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
  {
    return 0;
  }
  while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
  {
    i++;
  }
  return i;
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