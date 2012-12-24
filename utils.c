#include <stdlib.h>
#include <string.h>

char *join(const char *a, const char *b, char token)
{
    int size;
    char *new;

    size = strlen(a) + strlen(b) + 2;
    new = malloc(sizeof(char) * size);

    strcpy(new, a);
    new[strlen(a)] = token;
    strcpy(new + strlen(a) + 1, b);
    new[size - 1] = '\0';

    /* don't forget free this pointer! */
    return new;
}
