// Ecole supérieur MINF TP
// Manipulation MenuGen
// Créé le 9 Mai 2006 CHR
// Version 2013  du 19.11.2013 

// Definition pour le menuGen
// Rem inclure type_def.h avant d'inclure DefMenuGen.h

#ifndef DEFMENUGEN_H

#define DEFMENUGEN_H


typedef enum  { SignalCarre, SignalTriangle, SignalSinus, SignalDentDeScie } E_FormesSignal;

// Structure des paramètres du générateur
typedef struct {
      E_FormesSignal Forme;
      sint16 Frequence;
      sint16 Amplitude;
      sint16 Offset;
      uint32 Magic;
   } S_ParamGen;


#endif
