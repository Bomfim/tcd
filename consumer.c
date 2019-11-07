#include <stdio.h>
#include <stdlib.h>
#include "lib/ipclib.h"

int main()
{
    char *res, *s;
    res = receiveS();


    //res = receiveA();

    for (s = res; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');

    return 0;
}