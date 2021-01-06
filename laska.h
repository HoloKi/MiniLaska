
#ifndef LASKA_LASKA_H
#define LASKA_LASKA_H

#endif //LASKA_LASKA_H

#include <stddef.h>

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
struct tower{
    int composition[3];
};

typedef struct tower tower_t;


/** \defgroup CreaScacchiera
 * Funzioni che creano la scacchiera base
 * @{
 * */

/** \fn void riempi(tower_t **scacchiera,int size)\n
 * Funzione che mi posiziona e riempie le pedine iniziali\n
 * @param scacchiera = puntatore a scacchiera
 * @param size = dimensione x*x della scacchiera
 * */
void riempi(tower_t **scacchiera,int size);


/** \fn tower_t **mallocbase(int a,int b)\n
 * @param a = dimensione della prima malloc (7 poichè scacchiera 7*7)
 * @param b = seconda malloc (7 scacchiera 7*7)
 * */
tower_t **mallocbase(int a,int b);


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
 * @return **0** nel caso non fosse possibile un'azione, **1** nel caso fosse possibile l'azione.
 * */
int controllodestinazione(tower_t **scacchiera,int a,int b,int x, int y,int player);


/** \fn controllogrado(tower_t **scacchiera,int a,int b)
 * Funzione che controlla la posizione piu alta della pedina e mi restituisce la posizione.
 *
 * @param scacchiera = puntatore alla scacchiera
 * @param a = posizione x di (x,y) dove è presente la pedina
 * @param b = posizione y di (x,y) dove è presente la pedina
 * @return Ritorna **da 0 a 2**, cioè il valore dove è presente una pedina diversa 0
 *
 * */
int controllogrado(tower_t **scacchiera,int a,int b);


/** \fn controllo_pedine_presenti(tower_t** scacchiera)
 * funzione che mi dice se nella scacchiera comandano solo pedine dello stesso giocatore
 *
 * @param scacchiera = puntatore alla scacchiera
 * @return Ritorna **da 0 a 2**, se il valore ritornato è 1 oppure 2 allora vuol dire che uno dei due giocatori ha vinto
 *
 * */
int controllo_pedine_presenti(tower_t** scacchiera);


/**\fn controllopedina(tower_t** scacchiera, int a, int b, int player)
 * Funzione che fa il controllo sulla posizione della pedina.
 * @see controllogrado()
 * @param scacchiera = puntatore a scacchiera
 * @param a = valore x di (x,y) dove è presente la pedina
 * @param b = valore y di (x,y) dove è presente la pedina
 * @param player = valore da 1 a 4 dove indica il giocatore
 * @return **0** nel caso non rispetti i controlli, altrimenti **1**
 * */
int controllopedina(tower_t** scacchiera, int a, int b, int player);


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
void spostapedina(tower_t **scacchiera,int a,int b,int x,int y);


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
int controllamangio(tower_t **scacchiera,int a,int b,int x,int y,int player);


/**\fn mangia(tower_t **scacchiera,int a,int b,int x,int y,int grado)
 * Funzione che mangia la pedina e scambia i valori
 * @param scacchiera = puntatore a scacchiera
 * @param a = x di (x,y) dove è presente la pedina
 * @param b = y di (x,y) dove è presente la pedina
 * @param x = x di (x,y) dove si sposterà la pedina
 * @param y = y di (x,y) dove si sposterà la pedina
 * @param grado = grado maggiore della pedina che mangia
 * */
void mangia(tower_t **scacchiera,int a,int b,int x,int y,int grado);


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
int angolopedine(tower_t **scacchiera,int a,int b,int player);


/**\fn obbligomangiare(tower_t **scacchiera,int player)
 * Funzione che controlla ogni signola pedina di un giocatore per vedere se può mangiare o no una pedina
 * @param scacchiera = puntatore alla scacchiera
 * @param player = giocatore
 * @return 1 se può mangiare 0 nel caso non debba mangiare
 * */
int obbligomangiare(tower_t **scacchiera,int player);


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
int movimentodue(tower_t **scacchiera,int a,int b,int x,int y);


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
void printbase(tower_t **scacchiera);


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
int movimentouno(tower_t **scacchiera,int a,int b,int x,int y);

/**@}
 * */

/** \fn turno(tower_t** scacchiera, int conta)
 * Funzione ricorsiva che gestisce i turni dei giocatori ed eventiali vincite
 *
 * @param scacchiera = puntatore a scacchiera
 * @param conta = indica se è il turno del player1 oppure del player2
 *
 * */
int turno(tower_t** scacchiera, int conta);

/**\fn libera(tower_t **scacchiera)
 * @param scacchiera = puntatore a scacchiera
 * Funzione che libera la memoria
 * */
void libera(tower_t **scacchiera);