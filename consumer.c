#include <stdio.h>
#include <stdlib.h>
#include "lib/ipclib.h"

int main()
{
    char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";

    receiveS(alfabeto);
    // int status = receiveA(alfabeto);

    // if (status == -1)
    //     printf("%d\n", status);

    return 0;
}