#include "laska.h"
#include <stdio.h>

int menu(void){
    int a;
    printf("Benvenuto in laska!\n\n");
    printf("Premi il numero desiderato per scegliere un opzione\n");
    printf("1) Come giocare\n");
    printf("2) Giocatore vs Giocatore\n");

    scanf("%d",&a);
    if(a==1){
        printf("Giocare a Laska è come giocare a dama. A turno ogni giocatore deve spostare una delle sue pedine nelle caselle in avanti poste diagonalmente.\n");
        printf("Attento, devi mangiare quando possibile. Inoltre quando mangi una pedina,la pedina mangiata si posizionerà sotto la tua formando una torre di altezza max 3.\n");
        printf("Quando raggiunto l'altezza massima, l'ultima pedina verra sostituita con la pedina più bassa. Inoltre se mangi un'altra torre, mangerai solo la pedina più alta.\n");
        printf("La pedina più alta determina il giocatore che può controllarlo. Raggiunta la parte superiore della scacchiera, la pedina potrà anche muoversi indietro.\n");
        printf("Vince quando un giocatore è impossibilitato a muoversi o non ha pedine a disposizione");
        return 3;
    }else{
        if(a==2){
            return 1;
        }else{
            printf("Non c'è alcuna opzione relativo a questo numero!");
            return 0;
        }
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