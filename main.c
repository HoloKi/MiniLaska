#include "laska.h"

int main(){
    int conta=0;
    tower_t **scacchiera;
    scacchiera = mallocbase(7,7);
    riempi(scacchiera,7);
    printbase(scacchiera);
    turno(scacchiera, conta);

    return 0;
}