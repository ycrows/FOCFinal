#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *start;

    if (str)
        *saveptr = str;
    if (*saveptr == NULL)
        return NULL;

    start = *saveptr;
    start += strspn(start, delim);
    if (*start == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    char *end = start + strcspn(start, delim);
    if (*end)
        *end++ = '\0';
    *saveptr = end;
    return start;
}
