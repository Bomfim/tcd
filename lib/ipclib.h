#ifndef _IPCLIB_H_
#define _IPCLIB_H_

#define MAXSIZE 28
#define KEY 5678

extern void sendS(char c[]);
extern char *receiveS();
extern void sendA(char c[]);
extern char *receiveA();

#endif