#include "../lib/stdlib.h"

server_t* globServ;

void readl(char* ln) {
    int ab = 0;
    while (1) {
        char ky = globServ->inputCon();
        globServ->teletypeCon(ky);
        if (ky == '\n') {
            ln[ab] = 0;
            return;
        }
        else if (ky == '\b') {
            ln[ab--] = 0;
        }
        else {
            ln[ab++] = ky;
        }
    }
}

void _start(server_t* serv) {
    globServ = serv;

    *globServ->structCon.color = 0x0A;

    serv->printCon("apple banana");
    while(1) {
        char xd[20];
        serv->printCon("# ");
        readl(xd);
    }
}