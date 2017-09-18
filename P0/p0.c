#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int terminado = 0;
char comando[255];
char *comando_troceado[255];

int trocearCadena(char *cadena, char *trozos[])
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

void leer_entrada()
{
  fgets(comando, 255, stdin);
}

void salir()
{
  exit(0);
}

void imprimir_autores(const int numero_trozos)
{
  int i, nombres = 0, login = 0;
  for (i = 0; i < numero_trozos; i++)
  {
    if (strcmp("-l", comando_troceado[i]) == 0)
    {
      login = 1;
    }
    else if (strcmp("-n", comando_troceado[i]) == 0)
    {
      nombres = 1;
    }
      //TODO Opcion desconocida si parametro desconocido
  }

  if (!nombres && !login)
  {
    nombres = 1;
    login = 1;
  }

  if (nombres && login)
  {
    printf("Autores:\n");
    printf("________\n\n");
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

void imprimir_pid(const int numero_trozos)
{
  if (numero_trozos > 1 && strcmp("-p", comando_troceado[1]) == 0)
  {
    printf("%d\n", getppid());
  }
  else
  {
    printf("%d\n", getpid());
  }
  //TODO Opcion desconocida si troceado distinto de -p
}

void procesar_entrada()
{
  //printf("Ha ejecutado: %s\n", comando);
  int numero_trozos = trocearCadena(comando, comando_troceado);
  //printf("Numero de trozos: %d\n", numero_trozos);
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
      imprimir_autores(numero_trozos);
    }
    else if (strcmp("pid", comando_troceado[0]) == 0)
    {
      imprimir_pid(numero_trozos);
    }
  }
}

int main(void)
{
  while (!terminado)
  {
    imprimir_prompt();
    leer_entrada();
    procesar_entrada();
  }
}