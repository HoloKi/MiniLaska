#include "laska.h"
#include <stdio.h>

int menu(void){
    int a;
    printf("Benvenuto in laska!\n\n");
    printf("Premi il numero desiderato per scegliere un opzione\n");
    printf("1) Giocatore vs Giocatore\n");
    scanf("%d",&a);
    if(a==1){
        return 1;
    }else{
        printf("Non c'è nessuna opzione riguardo a questo numero\n");
        return 3;
    }
}

int main(){
    int a;
    int conta=0;
    tower_t **scacchiera;
    scacchiera = mallocbase(7,7);
    riempi(scacchiera,7);
    a = menu();
    while(a==3){
        a = menu();
    }
    if(a==1){
        printbase(scacchiera);
        turno(scacchiera, conta);
    }else{
        return 0;
    }

    return 0;
}
