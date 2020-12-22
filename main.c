#include <stdio.h>
#include <stdlib.h>


/**FUNZIONE DEL GIOCO\n
 * Ad ogni pedina viene assegnato un numero che indica:\n
 * 1 = giocatore 1\n
 * 2 = giocatore 2\n
 * 3 = ufficiale (giocatore1)\n
 * 4 = ufficiale (giocatore2)\n
 * La pedina più alta visualizzata indica a di quale giocatore è l'intera torre.\n
 * */


/**Scruct tower\n
 * composto da @param composition[3] indica rispettivamente l'altezza massima della torre\n
 * composition[0] = 1/2 -> Disposizione iniziale delle pedine\n
 * composition[0] = 0 -> Disposizione delle caselle vuote o delle caselle #\n
 * Le successive slot di composition solo per le pedine mangiate. Nel caso rimanessero zero allora indica che è una casella vuota o #.\n
 * */
struct tower{              /*struttura per le caselle*/     /* compositon[3] indica la torre (in base ai valori 1, 2, 3, 4 possiamo capire se le pedine sottostanti sono di un detrminato giocatore e se sono promosse o meno)  */
    int composition[3];                                                     /* i 3 componenti della composition possono assumere i seguenti valori: 1 se la pedina è del player uno e di tipo semplice, */
};

typedef struct tower tower_t;                                             /* 2 se è una pedina del player 2 e del tipo semplice, 3 se è una pedina del player 1 ma promossa, 4 se è una pedina del player 2 ma promossa, */
/* e NULL se non c'è nessuna pedina */

/** \addtogroup CreaScacchiera
 * Funzioni che creano la scacchiera
 * @{
 * */

/**void riempi(tower_t **scacchiera,int size)\n
 * Funzione che mi posiziona e riempie le pedine iniziali\n
 * @param scacchiera = puntatore a scacchiera
 * @param size = dimensione x*x della scacchiera
 * */
/*funzione che mi posiziona le pedine da capo*/
void riempi(tower_t **scacchiera,int size){
    int i,k,conta=0;
    int pedina;
    for(i=0;i<size;i++) {
        if(conta<11){
            pedina=1;
        }else{
            if(conta>10 && conta < 13){
                pedina=0;
            }else{
                pedina=2;
            }
        }
        for (k = 0; k < size; k++) {
            if((i+k)%2==0){
                scacchiera[i][k].composition[0]=pedina;
                scacchiera[i][k].composition[1]=0;
                scacchiera[i][k].composition[2]=0;
                conta++;
            }else{
                /*#*/
                scacchiera[i][k].composition[0]=0;
                scacchiera[i][k].composition[1]=0;
                scacchiera[i][k].composition[2]=0;
            }
        }
    }
    scacchiera[3][1].composition[0]=0;
    scacchiera[3][3].composition[0]=0;
    scacchiera[3][5].composition[0]=0;
}


/**tower_t **mallocbase(int a,int b)\n
 * @param a = dimensione della prima malloc (7 poichè scacchiera 7*7)
 * @param b = seconda malloc (7 scacchiera 7*7)
 * */
tower_t **mallocbase(int a,int b){
    tower_t **base;
    int i;
    base = (tower_t **)malloc((b*a)*sizeof(tower_t *));
    for(i=0;i<b;i++){
        base[i] = (tower_t *)malloc(b*sizeof(tower_t));
    }
    return base;
}

/**
 * @}
 * */


int controllodestinazione(tower_t **scacchiera,int a,int b,int x, int y,int player){ /*per controllare se un azione è possibile*/
    if(x<0 || y<0 || x>=7 || y>=7){ /*caso fuori dalla scacchiera*/
        /*posizione al di fuori della scacchiera*/
        return 0;
    }else{
        if(b==y){
            /*Non puoi andare avanti ma solo in obliquo*/
            return 0;
        }else{
            if((x+y)%2 != 0){
                /*caso in cui vado in una casella #4 4 3*/
                return 0;
            }else {
                if (scacchiera[x][y].composition[0] != 0) { /*controllo che sia una casella vuota*/
                    /*Casella gia occupata!*/
                    return 0;
                } else {
                    if (abs(a - x) > 3 || abs(b - y) > 3) {
                        /*Stai saltando di troppe caselle!*/
                        return 0;
                    } else {
                        if (player == 2) {
                            if (a < x) {
                                /*Non puoi tornare indietro!*/
                                return 0;
                            }

                        } else {
                            if (player == 1) {
                                if (a > x) {
                                    /*Non puoi tornare indietro!*/
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}




int controllogrado(tower_t **scacchiera,int a,int b){ /*controllo il livello maggiore della torre*/
    if(a<0 || b<0 || a>=7 || b>=7) {/*caso fuori dalla scacchiera*/
        /*posizione al di fuori della scacchiera*/
        return 0;
    }else {
        if (scacchiera[a][b].composition[0] == 0) {
            return 0;
        } else {
            if (scacchiera[a][b].composition[1] == 0 && scacchiera[a][b].composition[0] != 0) {
                return 0;
            } else {
                if (scacchiera[a][b].composition[2] == 0 && scacchiera[a][b].composition[1] != 0) {
                    return 1;
                } else {
                    return 2;
                }
            }
        }
    }
}

int controllo_pedine_presenti(tower_t** scacchiera){        /*funzione che mi dice se nella scacchiera comandano solo pedine dello stesso giocatore*/
    int i, j, conta1 = 0, conta2 = 0;
    for(i=0; i<7; i++){
        for(j=0; j<7; j++) {
            if ((i+j) % 2 == 0) {
                if(scacchiera[i][j].composition[controllogrado(scacchiera,i, j)]==1 || scacchiera[i][j].composition[controllogrado(scacchiera,i, j)]==3)
                    conta1++;
                else{
                    if(scacchiera[i][j].composition[controllogrado(scacchiera,i, j)]==2 || scacchiera[i][j].composition[controllogrado(scacchiera,i, j)]==4)
                        conta2++;
                    else
                        continue;
                }
            }
            else
                continue;
        }
    }
    if(conta1 == 0){
        return 1;
    }
    else{
        if(conta2 == 0){
            return 2;
        }
        else
            return 0;
    }
}


int controllopedina(tower_t** scacchiera, int a, int b, int player) {
    int grado, giocatore;
    if(a<0 || b<0 || a>=7 || b>=7) {/*caso fuori dalla scacchiera*/
        /*posizione al di fuori della scacchiera*/
        return 0;
    }else {
        grado = controllogrado(scacchiera, a, b);
        giocatore = scacchiera[a][b].composition[grado];
        if ((a + b) % 2 != 0) { /*caso in cui incontra #*/
            /*Posizione non disponibile della scacchiera -> #*/
            return 0;
        } else {
            if (scacchiera[a][b].composition[0] == 0) { /*controllo che non sia una casella vuota*/
                /*Casella vuota!*/
                return 0;
            } else {
                if ((giocatore - player) != 0 && (giocatore - player) != 2) {
                    /*la pedina selezionata è dell'avversario!*/
                    return 0;
                } else
                    return 1;
            }
        }
    }
}




void spostapedina(tower_t **scacchiera,int a,int b,int x,int y){
    tower_t supp;
    int i;
    for(i=0;i<3;i++){
        supp.composition[i]=scacchiera[a][b].composition[i];
        scacchiera[a][b].composition[i]=scacchiera[x][y].composition[i];
        scacchiera[x][y].composition[i]=supp.composition[i];
    }

}



int controllamangio(tower_t **scacchiera,int a,int b,int x,int y,int player){
    int x1,x2,grado,giocatore;
    if(x<0 || y<0 || x>=7 || y>=7){
        return -1;
    }else {

        if (a - x > 0 && b - y < 0) { /*caso + -, es G(4,4) e F(2,6)*/
            x1 = a - 1;
            x2 = b + 1;
        } else {
            if (a - x > 0 && b - y > 0) { /*caso + +, es G(4,4) e F(2,2)*/
                x1 = a - 1;
                x2 = b - 1;
            } else {
                if (a - x < 0 && b - y < 0) { /*es G(4,4) e F(6,6)*/
                    x1 = a + 1;
                    x2 = b + 1;
                } else {
                    x1 = a + 1;
                    x2 = b - 1;
                }
            }
        }
        grado = controllogrado(scacchiera, x1, x2);
        giocatore = scacchiera[x1][x2].composition[grado];
        if (giocatore == 0) {
            /*Non puoi saltare su una casella vuota!*/
            return 0;
        } else {
            if (giocatore - player == 0 || giocatore - player == 2) {
                /*Non puoi saltare una tua stessa pedina!*/
                return 0;
            }
        }
    }
    return 1;
}


void mangia(tower_t **scacchiera,int a,int b,int x,int y,int grado){
    int x1,x2,gradox1;
    if(a-x>0 && b-y<0) { /*caso + -, es G(4,4) e F(2,6)*/
        x1= a-1;
        x2 = b+1;
    }else{
        if(a-x>0 && b-y>0) { /*caso + +, es G(4,4) e F(2,2)*/
            x1 = a - 1;
            x2 = b - 1;
        }else{
            if(a-x<0 && b-y<0){ /*es G(4,4) e F(6,6)*/
                x1= a + 1;
                x2 = b + 1;
            }else{
                x1 = a + 1;
                x2 = b - 1;
            }
        }
    }
    gradox1 = controllogrado(scacchiera,x1,x2);
    if(grado==2){ /*caso in cui la torre è alta 3*/
        scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1]; /*cambio (a.b) piu basso con la composizione piu alta di (x1,x2)*/
        scacchiera[x1][x2].composition[gradox1]=0;
        spostapedina(scacchiera,a,b,x,y);
    }else{
        if(grado==1){
            scacchiera[a][b].composition[2]=scacchiera[a][b].composition[1];
            scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
            scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
            scacchiera[x1][x2].composition[gradox1]=0;
            spostapedina(scacchiera,a,b,x,y);
        }else{
            scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
            scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
            scacchiera[x1][x2].composition[gradox1]=0;
            spostapedina(scacchiera,a,b,x,y);
        }
    }
}

/*controlla gli spazi adiacenti se può mangiare, se si è obbligato a spostarsi in quella o quelle caselle*/
int angolopedine(tower_t **scacchiera,int a,int b,int player){
    /*controllo se dopo una casella adiacente è presente una casella vuota*/
    /*ATTENTO, DEVO CONTROLLARE SE LA PEDINA PUò TORNARE INDIETRO E SE è IL GIOCATORE 1 O 2 POICHè DETERMINA IL CONTROLLO DELLE POSIZIONI*/
    int controllo1,controllo2,controllo3,controllo4;
    int angolo1,angolo2,angolo3,angolo4;
    if(player==3 || player ==4) {/*da correggere poichè non abbiamo settato l'ufficiale*/
        /*può tornare indietro*/
        controllo1 = controllodestinazione(scacchiera,a,b,a-2,b-2,player); /*1*/
        controllo2 = controllodestinazione(scacchiera,a,b,a-2,b+2,player);
        controllo3 = controllodestinazione(scacchiera,a,b,a+2,b-2,player);
        controllo4 = controllodestinazione(scacchiera,a,b,a+2,b+2,player);
        angolo1 = controllamangio(scacchiera,a,b,a-2,b-2,player); /*angolo tra controllo 1 e (a,b)*/ /*mi deve dare 0 poichè deve esserci un numero*/
        angolo2 = controllamangio(scacchiera,a,b,a-2,b+2,player); /*controllo2*/
        angolo3 = controllamangio(scacchiera,a,b,a+2,b-2,player); /*controllo3*/
        angolo4 = controllamangio(scacchiera,a,b,a+2,b+2,player); /*controllo4*/
        if((controllo1!=0 && angolo1==1 )|| (controllo2!=0 && angolo2==1) || (controllo3!=0&& angolo3==1) || (controllo4!=0 && angolo4 ==1)){
            return 1;
        }else{
            return 0;
        }
    }else{
        if(player==2){ /*&& non ufficiale da correggere*/
            controllo1 = controllodestinazione(scacchiera,a,b,a-2,b-2,2); /*1*/
            controllo2 = controllodestinazione(scacchiera,a,b,a-2,b+2,2);
            angolo1 = controllamangio(scacchiera,a,b,a-2,b-2,2); /*angolo tra controllo 1 e (a,b)*/ /*mi deve dare 0 poichè deve esserci un numero*/
            angolo2 = controllamangio(scacchiera,a,b,a-2,b+2,2); /*controllo2*/
            if((controllo1!=0 && angolo1==1) || (controllo2!=0 && angolo2==1)){
                return 1;
            }else{
                return 0;
            }
        }else{
            if(player==1){/*&& non ufficiale*/
                controllo1 = controllodestinazione(scacchiera,a,b,a+2,b-2,1);
                controllo2 = controllodestinazione(scacchiera,a,b,a+2,b+2,1);
                angolo1 = controllamangio(scacchiera,a,b,a+2,b-2,1); /*angolo tra controllo 1 e (a,b)*/ /*mi deve dare 0 poichè deve esserci un numero*/
                angolo2 = controllamangio(scacchiera,a,b,a+2,b+2,1); /*controllo2*/
                if((controllo1!=0 && angolo1==1) || (controllo2!=0 && angolo2==1)){
                    return 1;
                }else{
                    return 0;
                }
            }
        }
    }
    return 0;
}

int obbligomangiare(tower_t **scacchiera,int player){
    int i,k,grado,giocatore,check,flag=0;
    for(i=0;i<7;i++){
        for(k=0;k<7;k++){
            grado = controllogrado(scacchiera,i,k);
            giocatore = scacchiera[i][k].composition[grado]; /*mi da il giocatore della pedina piu alta*/
            if((giocatore-player==0) || (giocatore-player)==2){  /*giocatore 2*/
                check = angolopedine(scacchiera,i,k,giocatore);
                if(check==1){
                    printf("Player %d puoi mangiare in posizione (%d,%d)!\n", player,i,k);
                    flag=1;
                }
            }
        }
    }
    if(flag==1){
        return 1;
    }else{
        return 0;
    }
}


int movimentodue(tower_t **scacchiera,int a,int b,int x,int y) { /*(a,b)posizione 1, (x,y)posizione 2*/
    int grado,controllo,giocatore,c,f,g;

    grado = controllogrado(scacchiera,a,b); /*controllo che grado è la scacchiera da 0 a 2*/
    g = controllopedina(scacchiera,a,b,2);
    if(g==0 || grado==-1){
        printf("coordinate non valide, inseriscile correttamente\n");
        return 0;
    }else{
        giocatore = scacchiera[a][b].composition[grado]; /*trovo il valore della posizione piu alta della pedina*/
        if(giocatore==1 || giocatore ==3) {
            return 0;
        }else{
            controllo = controllodestinazione(scacchiera,a,b,x,y,giocatore); /*controlla che (x,y) è una destinazione valida*/
            if(controllo==0) {
                return 0; /*se non elude il controllo ritorna 0*/
            }else {
                if(abs(a-x)==1 && abs(b-y)==1){ /*salto di una casella*/
                    c = obbligomangiare(scacchiera,2);
                    if(c==1){
                        printf("Hai una pedina da mangiare!\n");
                        return 0;
                    }else{
                        if(x==0 && giocatore==2){
                            spostapedina(scacchiera,a,b,x,y);
                            scacchiera[x][y].composition[grado]=4;
                            return 1;
                        }else{
                            spostapedina(scacchiera,a,b,x,y);
                            return 1;
                        }
                    }
                }else{
                    if(abs(a-x)==2 && abs(b-y)==2){ /*salto di due caselle quindi sto mangiando una pedina*/
                        f =  controllamangio(scacchiera,a,b,x,y,2);
                        if(f==0){
                            return 0;
                        }else{
                            if(x==0 && giocatore==2){
                                mangia(scacchiera,a,b,x,y,grado);
                                grado = controllogrado(scacchiera,x,y);
                                scacchiera[x][y].composition[grado]=4; /*cambio la composizione più alta in ufficiale*/
                                return 1;
                            }else{
                                mangia(scacchiera,a,b,x,y,grado);
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

void printbase(tower_t **scacchiera) {
    int i, k, z;
    printf("      0    1    2    3    4    5    6\n");
    printf("   +-----------------------------------+\n");
    for (i = 0; i < 7; i++) {
        for (z = 2; z >= 0; z--) {
            for (k = 0; k < 7; k++) {
                if ((i + k) % 2 != 0) {
                    if (k == 0 && z == 1)
                        printf("%d  |# # #", i);
                    else {
                        if (k == 0 && z != 1)
                            printf("   |# # #");
                        else {
                            if (k == 6)
                                printf("# # #|");
                            else
                                printf("# # #");
                        }
                    }

                } else {
                    if (k == 0 && z == 1) {
                        if(scacchiera[i][k].composition[z] == 0)
                            printf("%d  |     ", i);
                        else
                            printf("%d  |  %d  ", i, scacchiera[i][k].composition[z]);
                    }
                    else {
                        if (k == 0 && z != 1){
                            if(scacchiera[i][k].composition[z] == 0)
                                printf("   |     ");
                            else
                                printf("   |  %d  ", scacchiera[i][k].composition[z]);
                        }
                        else {
                            if (k == 6) {
                                if(scacchiera[i][k].composition[z] == 0)
                                    printf("     |");
                                else
                                    printf("  %d  |", scacchiera[i][k].composition[z]);
                            }
                            else {
                                if(scacchiera[i][k].composition[z] == 0)
                                    printf("     ");
                                else
                                    printf("  %d  ", scacchiera[i][k].composition[z]);
                            }
                        }
                    }
                }
            }
            printf("\n");
        }
    }
    printf("   +-----------------------------------+\n");
}


int movimentouno(tower_t **scacchiera,int a,int b,int x,int y) { /*(a,b)posizione 1, (x,y)posizione 2*/
    int grado,controllo,giocatore,c,f,g;
    g = controllopedina(scacchiera,a,b,1);
    grado = controllogrado(scacchiera,a,b); /*controllo che grado è la scacchiera da 0 a 2*/
    if(g==0 || grado == -1){
        printf("coordinate non valide, inseriscile correttamente\n");
        return 0;
    }else{
        giocatore = scacchiera[a][b].composition[grado]; /*trovo il valore della posizione piu alta della pedina*/
        if(giocatore==2 || giocatore ==4) {
            return 0;
        }else{
            controllo = controllodestinazione(scacchiera,a,b,x,y,giocatore); /*controlla che (x,y) è una destinazione valida*/
            if(controllo==0) {
                return 0; /*se non elude il controllo ritorna 0*/
            }else {
                if(abs(a-x)==1 && abs(b-y)==1){ /*salto di una casella*/
                    c = obbligomangiare(scacchiera,1);
                    if(c==1){
                        printf("Hai una pedina da mangiare!\n");
                        return 0;
                    }else{
                        if(x==6 && giocatore==1){
                            spostapedina(scacchiera,a,b,x,y);
                            scacchiera[x][y].composition[grado]=3;
                            return 1;
                        }else{
                            spostapedina(scacchiera,a,b,x,y);
                            return 1;
                        }
                    }
                }else{
                    if(abs(a-x)==2 && abs(b-y)==2){ /*salto di due caselle quindi sto mangiando una pedina*/
                        f =  controllamangio(scacchiera,a,b,x,y,1);
                        if(f==0){
                            return 0;
                        }else{
                            if(x==6 && giocatore==1){
                                mangia(scacchiera,a,b,x,y,grado);
                                grado = controllogrado(scacchiera,x,y);
                                scacchiera[x][y].composition[grado]=3; /*cambio la composizione più alta in ufficiale*/
                                return 1;
                            }else{
                                mangia(scacchiera,a,b,x,y,grado);
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int turno(tower_t** scacchiera, int conta){               /*funzione che gestisce il turno dei giocatori*/    /*SISTEMARE IL PROBLEMA CHE QUANDO PRENDO UNA PEDINA FUORI DALLA SCACCHIERA MI TERMINA IL PROGRAMMA*/
    int a, b, x, y;
    int controllo;
    while(controllo_pedine_presenti(scacchiera) == 0) {
        if (conta % 2 == 0) {
            printf("\n------DEBUG-------------------------------------------------------\n");
            printf("player 2 inserisci le coordinate\n");
            scanf("%d", &a);
            scanf("%d", &b);
            scanf("%d", &x);
            scanf("%d", &y);
            printf("Pedina (%d,%d) nella casella (%d,%d)\n", a, b, x, y);
            controllo = movimentodue(scacchiera, a, b, x, y);
            if (controllo == 0)
                turno(scacchiera, conta);
            else
                printbase(scacchiera);
            conta++;

        } else {
            printf("\n------DEBUG-------------------------------------------------------\n");
            printf("player 1 inserisci le coordinate\n");
            scanf("%d", &a);
            scanf("%d", &b);
            scanf("%d", &x);
            scanf("%d", &y);
            printf("Pedina (%d,%d) nella casella (%d,%d)\n", a, b, x, y);
            controllo = movimentouno(scacchiera, a, b, x, y);
            if (controllo == 0)
                turno(scacchiera, conta);
            else
                printbase(scacchiera);
            conta ++;
        }

    }
    if(controllo_pedine_presenti(scacchiera) == 1)
        printf("PLAYER 2 HA VINTO !!!");
    else
        printf("PLAYER 1 HA VINTO !!!");
    return 0;

}


int main(){
    int conta=0;
    int controllo=0;
    tower_t **scacchiera;
    scacchiera = mallocbase(7,7);
    riempi(scacchiera,7);
    printbase(scacchiera);
    turno(scacchiera, conta);

    return 0;
}

