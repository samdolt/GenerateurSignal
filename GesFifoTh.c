// ETML Ecole Technique
// Fichier GesFifoTh.c

// Exemple gestion d'un fifo de caractère, utilisation de pointeur et
// d'un descripteur de fifo

// Création : Christian HUBER      06/05/2003
// Modification : Christian HUBER  19/03/2007
//    Ajout possibilité init du FiFo avec valeur particulière

#include "type_def.h"

// structure décrivant un FIFO
typedef struct fifo {
   sint8 fifoSize;    // taille du fifo
   sint8 *pDebFifo;   // pointeur sur début du fifo
   sint8 *pFinFifo;   // pointeur sur fin du fifo
   sint8 *pWrite;     // pointeur d'écriture
   sint8 *pRead;      // pointeur de lecture
} S_fifo;


// prototypes des fonctions de gestion du fifo
uint8 putChar ( S_fifo *pDescrFifo, sint8 charToPut );
uint8 getChar ( S_fifo *pDescrFifo, sint8 *carLu );


/*---------------*/
/* InitFifo      */
/*===============*/

// Initialisation du descripteur de FIFO
void InitFifo ( S_fifo *pDescrFifo, sint8 FifoSize, sint8 *pDebFifo, sint8 InitVal )
{
   sint8 i;
   sint8 *pFif;
   pDescrFifo->fifoSize =   FifoSize;
   pDescrFifo->pDebFifo =   pDebFifo; // début du fifo
   // fin du fifo
   pDescrFifo->pFinFifo =   pDebFifo + (FifoSize - 1);
   pDescrFifo->pWrite   =   pDebFifo;  // début du fifo
   pDescrFifo->pRead    =   pDebFifo;  // début du fifo
   pFif = pDebFifo;
   
   // New remplissage initial du fifo
   for (i=0; i < FifoSize; i++) {
      *pFif  = InitVal;
      pFif++;
   }
} /* InitFifo */


/*---------------*/
/* GetWriteSpace */
/*===============*/

// Retourne la place disponible en écriture

sint8 GetWriteSpace ( S_fifo *pDescrFifo)
{
   sint8 writeSize;

   // Détermine le nb de car.que l'on peut déposer
   writeSize = pDescrFifo->pRead - pDescrFifo->pWrite -1;
   if (writeSize < 0) {
      writeSize = writeSize + pDescrFifo->fifoSize;
    }
   return (writeSize);
} /* GetWriteSpace */


/*-------------*/
/* GetReadSize */
/*=============*/

// Retourne le nombre de caractères à lire

sint8 GetReadSize ( S_fifo *pDescrFifo)
{
   sint8 readSize;

   readSize = pDescrFifo->pWrite - pDescrFifo->pRead;
   if (readSize < 0) {
       readSize = readSize +  pDescrFifo->fifoSize;
   }

   return (readSize);
} /* GetReadSize */

/*---------------*/
/* PutCharInFifo */
/*===============*/

// Dépose un caractère dans le FIFO
// Retourne 0 si OK, 1 si FIFO full

uint8 PutCharInFifo ( S_fifo *pDescrFifo, sint8 charToPut )
{
   uint8 writeStatus;

   // test si fifo est FULL
   if (getWriteSpace(pDescrFifo) == 0) {
      writeStatus = 1; // fifo FULL
   }
   else {
      // écrit le caractère dans le FIFO
      *(pDescrFifo->pWrite) = charToPut;

      // incrément le pointeur d'écriture
      pDescrFifo->pWrite++;
      // gestion du rebouclement
      if (pDescrFifo->pWrite > pDescrFifo->pFinFifo) {
          pDescrFifo->pWrite = pDescrFifo->pDebFifo;
      }

      writeStatus = 0; // OK
   }
   return (writeStatus);
} // PutCharInFifo 


/*-----------------*/
/* GetCharFromFifo */
/*=================*/

// Obtient (lecture) un caractère du fifo 
// retourne 0 si OK, 1 si empty
// le caractère lu est retourné par réference

uint8 GetCharFromFifo ( S_fifo *pDescrFifo, sint8 *carLu )
{
   sint8 readSize;
   uint8 readStatus;

   // détermine le nb de car. que l'on peut lire
   readSize = GetReadSize(pDescrFifo);

   // test si fifo est vide
   if (readSize == 0) {
      readStatus = 1; // fifo EMPTY
      *carLu = 0;     // carLu = NULL
   }
   else {
      // lis le caractère dans le FIFO
      *carLu = *(pDescrFifo->pRead);

      // incrément du pointeur de lecture
      pDescrFifo->pRead++;
      // gestion du rebouclement
      if (pDescrFifo->pRead > pDescrFifo->pFinFifo) {
          pDescrFifo->pRead = pDescrFifo->pDebFifo;
      }
      readStatus = 0; // OK
   }
   return (readStatus);
} // GetCharFromFifo 


