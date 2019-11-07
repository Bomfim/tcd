#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
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

    sem_t *semptr = sem_open("semaphore", /* name */
                             O_CREAT,     /* create the semaphore */
                             0644,        /* protection perms */
                             0);          /* initial value */
    if (semptr == (void *)-1)
        die("sem_open");

    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;

    if (sem_post(semptr) < 0)
        die("sem_post");

    /*
     * Wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    sem_close(semptr);
    exit(0);
}

void receiveS()
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    /* create a semaphore for mutual exclusion */
    sem_t *semptr = sem_open("semaphore", /* name */
                             O_CREAT,     /* create the semaphore */
                             0644,        /* protection perms */
                             0);          /* initial value */
    if (semptr == (void *)-1)
        die("sem_open");

    if (!sem_wait(semptr))
    { /* wait until semaphore != 0 */
        int i;
        for (s = shm; *s != '\0'; s++)
            putchar(*s);
        putchar('\n');
        *shm = '*';
        sem_post(semptr);
    }
    sem_close(semptr);

    exit(0);
}