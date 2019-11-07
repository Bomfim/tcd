#ifndef _IPCLIB_H_
#define _IPCLIB_H_

#define MAXSIZE 27
#define KEY 9876

extern void sendS(char c[]);
extern void receiveS(char c[]);
extern void sendA(char c[]);
extern int receiveA(char c[]);

#endif