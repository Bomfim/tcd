#include <stdio.h>
#include <stdlib.h>
#include "lib/ipclib.h"

int main()
{
    char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
    
    sendS(alfabeto);
    // sendA(alfabeto);

    return 0;
}