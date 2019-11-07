#!/bin/bash
echo "Compiling IPCLIB"
gcc -o lib/ipclib.o -c lib/ipclib.c -fPIC -lpthread
gcc -shared -o lib/ipclib.so lib/ipclib.o -lm

echo "Compiling producer & consumer"

gcc producer.c -o producer lib/ipclib.o -lpthread
gcc consumer.c -o consumer lib/ipclib.o -lpthread

echo "Compilation successfully completed"