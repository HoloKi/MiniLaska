#include <stdio.h>
#include <stdlib.h>

#include "laska.h"

/**FUNZIONE DEL GIOCO\n
 * Ad ogni pedina viene assegnato un numero che indica:\n
 * 1 = giocatore 1\n
 * 2 = giocatore 2\n
 * 3 = ufficiale (giocatore1)\n
 * 4 = ufficiale (giocatore2)\n
 * La pedina più alta visualizzata indica a di quale giocatore è l'intera torre.\n
 * */


/** \defgroup CreaScacchiera
 * Funzioni che creano la scacchiera base
 * @{
 * */

/** \fn void riempi(tower_t **scacchiera,int size)\n
 * Funzione che mi posiziona e riempie le pedine iniziali\n
 * @param scacchiera = puntatore a scacchiera
 * @param size = dimensione x*x della scacchiera
 * */
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


/** \fn tower_t **mallocbase(int a,int b)\n
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

/** \defgroup Controlli
 *  Funzioni definite al controllo delle posizioni e delle pedine\n
 *  @{
 * */


/** \fn controllodestinazione(tower_t **scacchiera, int a, int b, int x ,int u , int player)
 * Funzione che controlla la destinazione della pedina.
 *
 * @param scacchiera = Puntatore alla scacchiera
 * @param a = posizione x di (x,y) dove è presente la pedina
 * @param b = posizione y di (x,y) dove è presente la pedina
 * @param x = posizione x di (x,y) dove sposto la pedina
 * @param y = posizione y di (x,y) dove sposto la pedina
 * @param player = giocatore
 * @return **0** nel caso non fosse possibile un'azione, **1** nel caso fosse possibile l'azione.
 * */
int controllodestinazione(tower_t **scacchiera,int a,int b,int x, int y,int player){ /*per controllare se un azione è possibile*/
    /** **Condizione in cui la destinazione è fuori dalla scacchiera.** */
    /**@code{.c}
     * if(x<0 || y<0 || x>=7 || y>=7){
     *      return 0;
     * }
     * @endcode*/
    if(x<0 || y<0 || x>=7 || y>=7){
        return 0;
    }else{
        /** **Condizione in cui non si muova in obliquo quindi solamente in avanti.**
         * @code{.c}
        if(b==y){
            return 0;
        }
            @endcode*/
        if(b==y){
            return 0;
        }else{
            /** **Condizione in cui si sposta in una casella # cioè una casella non accessibile.**
             * @code{.c}
             * if((x+y)%2 != 0){
                    return 0;
               }
             @endcode*/
            if((x+y)%2 != 0){
                return 0;
            }else {
                /** **Controllo che sia una casella vuota.**
                 * @code{.c}
                 * if (scacchiera[x][y].composition[0] != 0) {
                        return 0;
                   }
                 @endcode*/
                if (scacchiera[x][y].composition[0] != 0) {
                    return 0;
                } else {
                    /** **Controllo che non stia saltando oltre le 3 caselle.**
                     * @code{.c}
                     * if (abs(a - x) > 3 || abs(b - y) > 3) {
                            return 0;
                       }
                     @endcode*/
                    if (abs(a - x) > 3 || abs(b - y) > 3) {
                        return 0;
                    } else {
                        /** **Controllo che se è giocatore 1 o 2 non torni indietro.**
                         * @code{.c}
                         * if (player == 2) {
                                if (a < x) {
                                    return 0;
                                }
                            }
                            if (player == 1) {
                                if (a > x) {
                                    return 0;
                                }
                            }
                         @endcode*/
                        if (player == 2) {
                            if (a < x) {
                                return 0;
                            }

                        } else {
                            if (player == 1) {
                                if (a > x) {
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



/** \fn controllogrado(tower_t **scacchiera,int a,int b)
 * Funzione che controlla la posizione piu alta della pedina e mi restituisce la posizione.
 *
 * @param scacchiera = puntatore alla scacchiera
 * @param a = posizione x di (x,y) dove è presente la pedina
 * @param b = posizione y di (x,y) dove è presente la pedina
 * @return Ritorna **da 0 a 2**, cioè il valore dove è presente una pedina diversa 0
 *
 * */
int controllogrado(tower_t **scacchiera,int a,int b){ /*controllo il livello maggiore della torre*/
    /** **Controllo sempre che non mi indichi una pedina al di fuori della scacchiera,
     *  cosi non va a controllare in posizioni non allocate della memoria.**
     * @code{.c}
     * if(a<0 || b<0 || a>=7 || b>=7) {
            return 0;
       }
     @endcode*/
    if(a<0 || b<0 || a>=7 || b>=7) {
        return 0;
    }else {
        /** **Caso in cui è una casella vuota**
         * @code{.c}
         * if (scacchiera[a][b].composition[0] == 0) {
                return 0;
           }
         @endcode*/
        if (scacchiera[a][b].composition[0] == 0) {
            return 0;
        } else {
            /** **Caso in cui in in posizione 1 non ci sia nulla e in posizione 0 ci sia un numero.**
             * @code{.c}
             * if (scacchiera[a][b].composition[1] == 0 && scacchiera[a][b].composition[0] != 0) {
                    return 0;
                }
             @endcode*/
            if (scacchiera[a][b].composition[1] == 0 && scacchiera[a][b].composition[0] != 0) {
                return 0;
            } else {
                /** **Caso in cui in in posizione 2 non ci sia nulla e in posizione 1 ci sia un numero.**
                * @code{.c}
                * if (scacchiera[a][b].composition[2] == 0 && scacchiera[a][b].composition[1] != 0) {
                    return 1;
                }
             @endcode*/
                if (scacchiera[a][b].composition[2] == 0 && scacchiera[a][b].composition[1] != 0) {
                    return 1;
                } else {
                    /** **Nel caso in cui non si soddisfino gli altri casi, cioè nel caso
                     *  in cui ci sia un numero on composition[0] e composizion[1] e infine anche in compsosition[2].**
                     * @code
                     * return 2;
                     * @endcode
                     * */
                    return 2;
                }
            }
        }
    }
}


/** \fn controllo_pedine_presenti(tower_t** scacchiera)
 * funzione che mi dice se nella scacchiera comandano solo pedine dello stesso giocatore
 *
 * @param scacchiera = puntatore alla scacchiera
 * @return Ritorna **da 0 a 2**, se il valore ritornato è 1 oppure 2 allora vuol dire che uno dei due giocatori ha vinto
 *
 * */
int controllo_pedine_presenti(tower_t** scacchiera){
    int i, j, conta1 = 0, conta2 = 0;
    /** conta1 viene incrementato ogni volta che in una casella la pedina più alta appartiene al player1,
     *  conta2 invece viene incrementato ogni volta in cui in una casella la pedina più alta appartiene al player2 **/
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
    if(conta1 == 0){  /** caso in cui il player1 non ha pedine che comandano, ovvero non ha pedine presenti in cima ad alcuna torre **/
        return 1;
    }
    else{
        if(conta2 == 0){  /** caso in cui il player2 non ha pedine che comandano, ovvero non ha pedine presenti in cima ad alcuna torre **/
            return 2;
        }
        else
            return 0;
    }
}

/**\fn controllopedina(tower_t** scacchiera, int a, int b, int player)
 * Funzione che fa il controllo sulla posizione della pedina.
 * @see controllogrado()
 * @param scacchiera = puntatore a scacchiera
 * @param a = valore x di (x,y) dove è presente la pedina
 * @param b = valore y di (x,y) dove è presente la pedina
 * @param player = valore da 1 a 4 dove indica il giocatore
 * @return **0** nel caso non rispetti i controlli, altrimenti **1**
 * */
int controllopedina(tower_t** scacchiera, int a, int b, int player) {
    int grado, giocatore;
    /** **Controllo inanzitutto che la pedina non sia al di fuori della scacchiera.**
     *
     * @code{.c}
     * if(a<0 || b<0 || a>7 || b>7){
     *      return 0;
     * }
     * @endcode
     * */
    if(a<0 || b<0 || a>=7 || b>=7) {
        return 0;
    }else {
        grado = controllogrado(scacchiera, a, b);
        giocatore = scacchiera[a][b].composition[grado];
        /** **Controllo che non sia una posizione in cui non sia presente la pedina**
         * @code{.c}
         * if((a+b)%2!=0){
         *      return 0;
         * }
         * @endcode
         * */
        if ((a + b) % 2 != 0) {
            return 0;
        } else {
            /** **Controllo che non sia una casella vuota.**
             * @code{.c}
             * if (scacchiera[a][b].composition[0] == 0) {
             *      return 0;
             * }
             * @endcode
             * */
            if (scacchiera[a][b].composition[0] == 0) {
                return 0;
            } else {
                /**  **Controllo che la pedina sia quella del giocatore.**
                 * @code{.c}
                 * if ((giocatore - player) != 0 && (giocatore - player) != 2) {
                 *      return 0;
                 * }
                 * @endcode
                 * */
                if ((giocatore - player) != 0 && (giocatore - player) != 2) {
                    return 0;
                } else
                    return 1;
            }
        }
    }
}


/**
 * @}
 * */

/** \defgroup MangiaPedine
 *  Funzioni definite per mangiare la pedina e/o obbligo
 *  @{
 * */

/** \fn spostapedina(tower_t **scacchiera, int a, int b , int x, int y)
 * funzione che sposta le pedine nel caso dello spostamento di una sola casella vuota
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove si sposterà la pedina
 * @param y = y di (x,y) dove si sposterà la pedina
 * */
void spostapedina(tower_t **scacchiera,int a,int b,int x,int y){
    tower_t supp;
    int i;
    for(i=0;i<3;i++){
        supp.composition[i]=scacchiera[a][b].composition[i];
        scacchiera[a][b].composition[i]=scacchiera[x][y].composition[i];
        scacchiera[x][y].composition[i]=supp.composition[i];
    }

}


/**\fn controllamangio(tower_t **scacchiera,int a,int b,int x,int y,int player)
 * Funzione che controlla le possibili caselle da mangiare
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove si sposterà la pedina
 * @param y = y di (x,y) dove si sposterà la pedina
 * @param player = indica il giocatore
 * @return 1 se può mangiare 0 se non può
 * */
int controllamangio(tower_t **scacchiera,int a,int b,int x,int y,int player){
    int x1,x2,grado,giocatore;
    if(x<0 || y<0 || x>=7 || y>=7){
        return -1;
    }else {

        if (a - x > 0 && b - y < 0) {
            x1 = a - 1;
            x2 = b + 1;
        } else {
            if (a - x > 0 && b - y > 0) {
                x1 = a - 1;
                x2 = b - 1;
            } else {
                if (a - x < 0 && b - y < 0) {
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
        /**  **Controllo di non saltare una casella vuota o una mia stessa pedina.**
         * @code{.c}
         * if (giocatore == 0) {
         * return 0;
         * } else {
         * if (giocatore - player == 0 || giocatore - player == 2) {
         * @endcode
         * */
        if (giocatore == 0) {
            return 0;
        } else {
            if (giocatore - player == 0 || giocatore - player == 2) {
                return 0;
            }
        }
    }
    return 1;
}

/**\fn mangia(tower_t **scacchiera,int a,int b,int x,int y,int grado)
 * Funzione che mangia la pedina e scambia i valori
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove si sposterà la pedina
 * @param y = y di (x,y) dove si sposterà la pedina
 * @param grado = grado maggiore della pedina che mangia
 * */
void mangia(tower_t **scacchiera,int a,int b,int x,int y,int grado){
    int x1,x2,gradox1;
    /** **Controllo nelle posizioni oobliquie di 1 blocco**
     *
     * @code{.c}
     * if(a-x>0 && b-y<0) {
     *      x1= a-1;
     *      x2 = b+1;
     * }else{
     *     if(a-x>0 && b-y>0) {
     *         x1 = a - 1;
     *         x2 = b - 1;
     *     }else{
     *         if(a-x<0 && b-y<0){
     *             x1= a + 1;
     *             x2 = b + 1;
     *         }else{
     *             x1 = a + 1;
     *             x2 = b - 1;
     *         }
     *     }
     * }
     * @endcode
     * */
    if(a-x>0 && b-y<0) {
        x1= a-1;
        x2 = b+1;
    }else{
        if(a-x>0 && b-y>0) {
            x1 = a - 1;
            x2 = b - 1;
        }else{
            if(a-x<0 && b-y<0){
                x1= a + 1;
                x2 = b + 1;
            }else{
                x1 = a + 1;
                x2 = b - 1;
            }
        }
    }
    /** **Controllo l'altezza della pedina che mangio**\n
     * @see controllogrado()
     *
     * @code{.c}
     * gradox1 = controllogrado(scacchiera,x1,x2);
     * @endcode
     * */
    gradox1 = controllogrado(scacchiera,x1,x2);
    /** **Caso in cui la pedina che sto mangiando sia alto 3**\n
     * @see spostapedina()
     *
     * @code{.c}
     * if(grado==2){
     *      scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
     *      scacchiera[x1][x2].composition[gradox1]=0;
     *      spostapedina(scacchiera,a,b,x,y);
     * }
     * @endcode
     * */
    if(grado==2){
        scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
        scacchiera[x1][x2].composition[gradox1]=0;
        spostapedina(scacchiera,a,b,x,y);
    }else{
        /** **Caso in cui la pedina che sto mangiando sia alto 2**\n
         * @see spostapedina()
         *
         * @code{.c}
         * if(grado==1){
         *      scacchiera[a][b].composition[2]=scacchiera[a][b].composition[1];
         *      scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
         *      scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
         *      scacchiera[x1][x2].composition[gradox1]=0;
         *      spostapedina(scacchiera,a,b,x,y);
         * }
         * @endcode
         * */
        if(grado==1){
            scacchiera[a][b].composition[2]=scacchiera[a][b].composition[1];
            scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
            scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
            scacchiera[x1][x2].composition[gradox1]=0;
            spostapedina(scacchiera,a,b,x,y);
        }else{
            /** **Caso in cui mangio solamente una pedina**
             * @see spostapedina()
             *
             * @code{.c}
             *  scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
             *  scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
             *  scacchiera[x1][x2].composition[gradox1]=0;
             *  spostapedina(scacchiera,a,b,x,y);
             * @endcode
             * */
            scacchiera[a][b].composition[1]=scacchiera[a][b].composition[0];
            scacchiera[a][b].composition[0]=scacchiera[x1][x2].composition[gradox1];
            scacchiera[x1][x2].composition[gradox1]=0;
            spostapedina(scacchiera,a,b,x,y);
        }
    }
}


/**\fn angolopedine(tower_t **scacchiera,int a,int b,int player)
 * Funzione che controlla le posizioni dove potrebbe mangiare la pedina
 * @see controllodestinazione()
 * @see controllomangio()
 *
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param player = giocatore
 * @return 0 se non può mangiare alcuna pedina, 1 se può mangiare una pedina
 * */
int angolopedine(tower_t **scacchiera,int a,int b,int player){
    /** **Controllo nel caso la pedina sia un ufficiale e possa mangiare sia avanti che dietro**
     * @code{.c}
     *
     *  if(player==3 || player ==4) {
     *      controllo1 = controllodestinazione(scacchiera,a,b,a-2,b-2,player);
     *      controllo2 = controllodestinazione(scacchiera,a,b,a-2,b+2,player);
     *      controllo3 = controllodestinazione(scacchiera,a,b,a+2,b-2,player);
     *      controllo4 = controllodestinazione(scacchiera,a,b,a+2,b+2,player);
     *      angolo1 = controllamangio(scacchiera,a,b,a-2,b-2,player);
     *      angolo2 = controllamangio(scacchiera,a,b,a-2,b+2,player);
     *      angolo3 = controllamangio(scacchiera,a,b,a+2,b-2,player);
     *      angolo4 = controllamangio(scacchiera,a,b,a+2,b+2,player);
     *      if((controllo1!=0 && angolo1==1 )|| (controllo2!=0 && angolo2==1) || (controllo3!=0&& angolo3==1) || (controllo4!=0 && angolo4 ==1)){
     *          return 1;
     *      }else{
     *          return 0;
     *      }
     * }
     *
     * @endcode
     *
     * */
    int controllo1,controllo2,controllo3,controllo4;
    int angolo1,angolo2,angolo3,angolo4;
    if(player==3 || player ==4) {
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
        /** **Giocatore 2, simile è il codice per il giocatore 1**
         * @code{.c}
         * if(player==2){
         *      controllo1 = controllodestinazione(scacchiera,a,b,a-2,b-2,2);
         *      controllo2 = controllodestinazione(scacchiera,a,b,a-2,b+2,2);
         *      angolo1 = controllamangio(scacchiera,a,b,a-2,b-2,2);
         *      angolo2 = controllamangio(scacchiera,a,b,a-2,b+2,2);
         *      if((controllo1!=0 && angolo1==1) || (controllo2!=0 && angolo2==1)){
         *          return 1;
         *      }else{
         *          return 0;
         *      }
         *  }
         * @endcode
         * */
        if(player==2){
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
            if(player==1){
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

/**\fn obbligomangiare(tower_t **scacchiera,int player)
 * Funzione che controlla ogni signola pedina di un giocatore per vedere se può mangiare o no una pedina
 * @param scacchiera = puntatore alla scacchiera
 * @param player = giocatore
 * @return 1 se può mangiare 0 nel caso non debba mangiare
 * */
int obbligomangiare(tower_t **scacchiera,int player){
    int i,k,grado,giocatore,check,flag=0;
    for(i=0;i<7;i++){
        for(k=0;k<7;k++){
            grado = controllogrado(scacchiera,i,k);
            giocatore = scacchiera[i][k].composition[grado]; /*mi da il giocatore della pedina piu alta*/
            if((giocatore-player==0) || (giocatore-player)==2){  /*giocatore 2*/
                check = angolopedine(scacchiera,i,k,giocatore);
                if(check==1){
                    printf("Player %d puoi mangiare con la pedina in posizione (%d,%d)!\n", player,i,k);
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
/**
 * @}
 * */


/**\defgroup Movimento
 * Funzioni per definire il movimento delle pedine
 * @{
 * */

/**\fn movimentodue(tower_t **scacchiera,int a,int b,int x,int y)
 * Funzione che determina il movimento del giocatore 2
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove voglio che vada la pedina
 * @param y = y di (x,y) dove voglio che vada la pedina
 * @return 1 se va a buon fine, 0 se non è possibile efettuare la mossa
 * */
int movimentodue(tower_t **scacchiera,int a,int b,int x,int y) { /*(a,b)posizione 1, (x,y)posizione 2*/
    int grado,controllo,giocatore,c,f,g;
    /** **Controllo la pedina**
     * @see controllopedina()
     * @see controllogrado()
     *
     * @code{.c}
     * grado = controllogrado(scacchiera,a,b);
     * g = controllopedina(scacchiera,a,b,2);
     * if(g==0 || grado==-1){
     *      printf("coordinate non valide, inseriscile correttamente\n");
     *      return 0;
     * }
     * @endcode
     * */
    grado = controllogrado(scacchiera,a,b); /*controllo che grado è la scacchiera da 0 a 2*/
    g = controllopedina(scacchiera,a,b,2);
    if(g==0 || grado==-1){
        printf("coordinate non valide, inseriscile correttamente\n");
        return 0;
    }else{
        /** **Controllo che la pedina sia il giocatore giusto, controllo aggiuntivo**
         * @code{.c}
         * giocatore = scacchiera[a][b].composition[grado];
         * if(giocatore==1 || giocatore ==3) {
         *      return 0;
         * }
         * @endcode
         * */
        giocatore = scacchiera[a][b].composition[grado]; /*trovo il valore della posizione piu alta della pedina*/
        if(giocatore==1 || giocatore ==3) {
            return 0;
        }else{
            /** **Controllo la destinazione della pedina**
             * @see controllodestinazione()
             *
             * @code{.c}
             * controllo = controllodestinazione(scacchiera,a,b,x,y,giocatore);
             * if(controllo==0) {
             *      return 0;
             * }
             * @endcode
             * */
            controllo = controllodestinazione(scacchiera,a,b,x,y,giocatore); /*controlla che (x,y) è una destinazione valida*/
            if(controllo==0) {
                return 0; /*se non elude il controllo ritorna 0*/
            }else {
                /** **Nel caso in cui vadano a buon fine i controlli e faccio un solo movimento di salto, controllo se sto evitando di mangiare pedine**
                 * @see obbligomangiare()
                 * @see spostapedina()
                 *
                 * @code{.c}
                 * if(abs(a-x)==1 && abs(b-y)==1){
                 *      c = obbligomangiare(scacchiera,2);
                 *      if(c==1){
                 *          printf("Hai una pedina da mangiare!\n");
                 *          return 0;
                 *      }else{
                 *          if(x==0 && giocatore==2){
                 *              spostapedina(scacchiera,a,b,x,y);
                 *              scacchiera[x][y].composition[grado]=4;
                 *              return 1;
                 *          }else{
                 *              spostapedina(scacchiera,a,b,x,y);
                 *              return 1;
                 *          }
                 *      }
                 * }
                 * @endcode
                 * */
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
                    /** **Caso in cui salto 2 posizioni cioè mangio una pedina**
                     * @see controllamangio()
                     * @see controllogrado()
                     * @see mangia()
                     *
                     * @code{.c}
                     * if(abs(a-x)==2 && abs(b-y)==2){
                     *      f =  controllamangio(scacchiera,a,b,x,y,2);
                     *      if(f==0){
                     *          return 0;
                     *      }else{
                     *          if(x==0 && giocatore==2){
                     *              mangia(scacchiera,a,b,x,y,grado);
                     *              grado = controllogrado(scacchiera,x,y);
                     *              scacchiera[x][y].composition[grado]=4;
                     *              return 1;
                     *          }else{
                     *              mangia(scacchiera,a,b,x,y,grado);
                     *              return 1;
                     *          }
                     *      }
                     * }
                     * @endcode
                     * */
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

/**
 * @}
 * */

/**\defgroup print
 * Funzione che printa tutto il gioco
 * @{
 * */

/**\fn printbase(tower_t **scacchiera)
 * Funzione che printa la scacchiera
 * @param scacchiera = puntatore scacchiera
 * */
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
                        if (k == 0)
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
                        if (k == 0){
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

/**@}
 * */

/**\addtogroup Movimento
 * @{
 * */

/**\fn movimentouno(tower_t **scacchiera,int a,int b,int x,int y)
 * Funzione che sposta la pedina uno, simile al movimentodue
 * @see movimentodue()
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove voglio che vada la pedina
 * @param y = y di (x,y) dove voglio che vada la pedina
 * @return 1 se va a buon fine, 0 se non è possibile efettuare la mossa
 * */
int movimentouno(tower_t **scacchiera,int a,int b,int x,int y) {
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



/** \fn turno(tower_t** scacchiera, int conta)
 * Funzione ricorsiva che gestisce i turni dei giocatori ed eventiali vincite
 * @param scacchiera = puntatore a scacchiera
 * @param conta = indica se è il turno del player1 oppure del player2
 * */
int turno(tower_t** scacchiera, int conta){
    int a, b, x, y;
    int controllo;
    while(controllo_pedine_presenti(scacchiera) == 0) {
            /** **caso in cui è il turno del player2**
             * @see movimentodue()
             * @see turno()
             * @see printbase()
             *
             * @code{.c}
             * else{
             * printf("\n------DEBUG-------------------------------------------------------\n");
             * printf("player 2 inserisci le coordinate\n");
             * scanf("%d", &a);
             * scanf("%d", &b);
             * scanf("%d", &x);
             * scanf("%d", &y);
             * printf("Pedina (%d,%d) nella casella (%d,%d)\n", a, b, x, y);
             * controllo = movimentodue(scacchiera, a, b, x, y);
             * if (controllo == 0)
             *      turno(scacchiera, conta);
             * else
             *      printbase(scacchiera);
             * conta ++;
             * }
             * @endcode
             *
             * */
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
            /** **caso in cui è il turno del player1**
             * @see movimentouno()
             * @see turno()
             * @see printbase()
             *
             * @code{.c}
             * else {
             * printf("\n------DEBUG-------------------------------------------------------\n");
             * printf("player 1 inserisci le coordinate\n");
             * scanf("%d", &a);
             * scanf("%d", &b);
             * scanf("%d", &x);
             * scanf("%d", &y);
             * printf("Pedina (%d,%d) nella casella (%d,%d)\n", a, b, x, y);
             * controllo = movimentouno(scacchiera, a, b, x, y);
             * if (controllo == 0)
             *      turno(scacchiera, conta);
             * else
             *      printbase(scacchiera);
             * conta ++;
             * }
             * @endcode
             * */
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
    if(controllo_pedine_presenti(scacchiera) == 1) {
        printf("PLAYER 2 HA VINTO !!!");
        free(scacchiera);
    }else{
        printf("PLAYER 1 HA VINTO !!!");
        free(scacchiera);
    }
    return 0;

}

/**@}
 * */


/**\defgroup LiberaMemoria
 * funzione che libera la memoria
 * @{
 * */

/**\fn libera(tower_t **scacchiera)
 * @param scacchiera = puntatore a scacchiera
 * Funzione che libera la memoria
 * */
void libera(tower_t **scacchiera){
    int i;
    for(i=0;i<7;i++){
        free(scacchiera[i]);
    }
    free(scacchiera);
}

/**
 * @}
 * */