#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ipclib.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

void sendS()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    /*
     *      * Put some things into the memory for the
     *        other process to read.
     *        */
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;

    /*
     * Wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    exit(0);
}

void receiveS(){
    int shmid;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        die("shmat");

    //Now read what the server put in the memory.
    for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');

    /*
     *Change the first character of the
     *segment to '*', indicating we have read
     *the segment.
     */
    *shm = '*';

    exit(0);
}