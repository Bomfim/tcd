#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ipclib.h"

//Função para tratar os erros.
void die(char *s)
{
    perror(s);
    exit(1);
}

void sendS(char c[])
{
    int shmid, i;  // identificador da memória compartilhada;
    key_t key;     // chave única;
    char *shm, *s; // ponteiro para a memória compartihada; ponteiro para char;

    key = KEY;

    // shmget(chave, tamanho, Flag de criação/permissão na MC)
    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");

    // Antes de poder usar uma MC, você deve se acoplar a ele usando
    //shmat(id da MC, Endereço (NULL o SO decide onde vai alocar), Flags gerais).
    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    //Criação do semáforo.
    sem_t *semptr = sem_open("semaphore", /* nome */
                             O_CREAT,     /* Flag de criação */
                             0644,        /* Permissões de proteção */
                             0);          /* Valor inicial */
    if (semptr == (void *)-1)
        die("sem_open");

    //copia do endereço de shm para s e devida escrita na MC.
    s = shm;

    *s = '@';
    s++;
    for (i = 0; c[i] != '\0'; i++)
        *s++ = c[i];

    // Liga o semáforo para que o consumidor possa ler da MC.
    if (sem_post(semptr) < 0)
        die("sem_post");

    /*
     * Aguarda até o outro processo
     * alterar o primeiro caractere da nossa memória
     * para '*', indicando que leu o que
     * nós colocamos lá.
     */
    while (*shm != '*')
        sleep(1);
}

char *receiveS(char c[])
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    sem_t *semptr = sem_open("semaphore",
                             O_CREAT,
                             0644,
                             0);
    if (semptr == (void *)-1)
        die("sem_open");

    if (!sem_wait(semptr))
    { /* espera até o semáforo ser != 0 */

        /*
        * Aguarda até o outro processo
        * alterar o primeiro caractere da nossa memória
        * para '*@', indicando que escreveu.
        */
        while (*shm != '@')
            sleep(1);

        *shm = '*';
        sem_post(semptr);  // Chaveia o semáforo.
        sem_close(semptr); // Fecha o semáforo.
        return shm += 1;
    }

    //Desacoplamento da MC.
    // shmdt(shm);

    // Destroi a MC.
    // shmctl(shmid, IPC_RMID, NULL);
}

void sendA(char c[])
{
    int shmid, i;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    sem_t *semptr = sem_open("semaphore",
                             O_CREAT,
                             0644,
                             0);
    if (semptr == (void *)-1)
        die("sem_open");

    s = shm;

    *s = '@';
    s++;
    for (i = 0; c[i] != '\0'; i++)
        *s++ = c[i];

    if (sem_post(semptr) < 0)
        die("sem_post");
}

char *receiveA(char c[])
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = KEY;

    if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        die("shmat");

    sem_t *semptr = sem_open("semaphore",
                             O_CREAT,
                             0644,
                             0);
    if (semptr == (void *)-1)
        die("sem_open");

    if (!sem_wait(semptr))
    {
        // Essa lógica existe para que o consumidor nunca feche o semáforo para si mesmo.
        // Uma vez que não há nada de novo na MC ('*') Ele chaveia o semáforo e retorna -1 como especificado.
        if (*shm == '*')
        {
            sem_post(semptr);
            sem_close(semptr);
            return "-1";
        }

        *shm = '*';
        sem_post(semptr);
        sem_close(semptr);
        return shm += 1;
    }
}