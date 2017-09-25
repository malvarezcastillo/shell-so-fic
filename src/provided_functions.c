#include "provided_functions.h"
#include <string.h>
#include <stdlib.h>

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

char tipo_fichero(mode_t m)
{
    switch (m & S_IFMT)
    {
    case S_IFSOCK:
        return 's'; /*socket */
    case S_IFLNK:
        return 'l'; /*symbolic link*/
    case S_IFREG:
        return '-'; /*fichero normal*/
    case S_IFBLK:
        return 'b'; /*block device*/
    case S_IFDIR:
        return 'd'; /*directorio */
    case S_IFCHR:
        return 'c'; /*char device*/
    case S_IFIFO:
        return 'p'; /*pipe*/
    default:
        return '?'; /*desconocido, no deberia aparecer*/
    }
}

char *convierte_modo(mode_t m, char *permisos)
{
    strcpy(permisos, "---------- ");
    permisos[0] = tipo_fichero(m);
    if (m & S_IRUSR)
    {
        permisos[1] = 'r'; /*propietario*/
    }
    if (m & S_IWUSR)
    {
        permisos[2] = 'w';
    }
    if (m & S_IXUSR)
    {
        permisos[3] = 'x';
    }
    if (m & S_IRGRP)
    {
        permisos[4] = 'r'; /*grupo*/
    }
    if (m & S_IWGRP)
    {
        permisos[5] = 'w';
    }
    if (m & S_IXGRP)
    {
        permisos[6] = 'x';
    }
    if (m & S_IROTH)
    {
        permisos[7] = 'r'; /*resto*/
    }
    if (m & S_IWOTH)
    {
        permisos[8] = 'w';
    }
    if (m & S_IXOTH)
    {
        permisos[9] = 'x';
    }
    if (m & S_ISUID)
    {
        permisos[3] = 's'; /*setuid*/
    }
    if (m & S_ISGID)
    {
        permisos[6] = 's'; /*setgid*/
    }
    if (m & S_ISVTX)
    {
        permisos[9] = 't'; /*stickybit*/
    }
    return permisos;
}

char *convierte_modo2(mode_t m)
{
    static char permisos[12];
    strcpy(permisos, "---------- ");
    permisos[0] = tipo_fichero(m);
    if (m & S_IRUSR)
    {
        permisos[1] = 'r'; /*propietario*/
    }
    if (m & S_IWUSR)
    {
        permisos[2] = 'w';
    }
    if (m & S_IXUSR)
    {
        permisos[3] = 'x';
    }
    if (m & S_IRGRP)
    {
        permisos[4] = 'r'; /*grupo*/
    }
    if (m & S_IWGRP)
    {
        permisos[5] = 'w';
    }
    if (m & S_IXGRP)
    {
        permisos[6] = 'x';
    }
    if (m & S_IROTH)
    {
        permisos[7] = 'r'; /*resto*/
    }

    if (m & S_IWOTH)
    {
        permisos[8] = 'w';
    }
    if (m & S_IXOTH)
    {
        permisos[9] = 'x';
    }
    if (m & S_ISUID)
    {
        permisos[3] = 's'; /*setuid*/
    }
    if (m & S_ISGID)
    {
        permisos[6] = 's'; /*setgid*/
    }
    if (m & S_ISVTX)
    {
        permisos[9] = 't'; /*stickybit*/
    }
    return (permisos);
}
char *convierte_modo3(mode_t m)
{
    char *permisos;
    permisos = (char *)malloc(12);
    strcpy(permisos, "---------- ");
    permisos[0] = tipo_fichero(m);
    if (m & S_IRUSR)
    {
        permisos[1] = 'r'; /*propietario*/
    }
    if (m & S_IWUSR)
    {
        permisos[2] = 'w';
    }
    if (m & S_IXUSR)
    {
        permisos[3] = 'x';
    }
    if (m & S_IRGRP)
    {
        permisos[4] = 'r'; /*grupo*/
    }
    if (m & S_IWGRP)
    {
        permisos[5] = 'w';
    }
    if (m & S_IXGRP)
    {
        permisos[6] = 'x';
    }
    if (m & S_IROTH)
    {
        permisos[7] = 'r'; /*resto*/
    }
    if (m & S_IWOTH)
    {
        permisos[8] = 'w';
    }
    if (m & S_IXOTH)
    {
        permisos[9] = 'x';
    }
    if (m & S_ISUID)
    {
        permisos[3] = 's'; /*setuid*/
    }
    if (m & S_ISGID)
    {
        permisos[6] = 's'; /*setgid*/
    }
    if (m & S_ISVTX)
    {
        permisos[9] = 't'; /*stickybit*/
    }
    return (permisos);
}
