// fdebounce.c
// C. HUBER    23/11/2005

// C. HUBER    05/06/2013 Re ecriture pour contourner
//             Probl�me nouveau compilateur 4.140
//             Utilisation structure champs de bits

// Fonctions pour la gestion de l'anti-rebond simplifi�e d'un switch
//
//
// Principe : Il est n�cessaire d'appeler cycliquement la fonction DoDebounce
//            pour chaque switch. Suggestion cycle de 1 ms
//
//            Pour la gestion du switch, il y a 3 fonctions � disposition :
//       DebounceGetInput  fourni l'�tat du switch apr�s anti-rebond
//       DebounceIsPressed    true indique que l'on vient de presser la touche
//       DebounceIsReleased   true indique que l'on vient de relacher la touche
//       DebounceClearPressed    annule indication de pression sur la touche
//       DebounceClearReleased   annule indication de relachement de la touche
//
//  Remarques : toute les fonctions utilisent un descripteur de touche
//              un �tat 0 indique une touche press�e
//
//---------------------------------------------------------------------------

// d�finitions des types qui seront utilis�s dans cette application

// Etats du syst�me antirebond
typedef enum {
      DebounceWaitChange,
      DebounceWaitStable,
} E_DebouceStates;

// New
typedef struct {
   int8  KeyPressed :1;              // �v�nement touche press�e
   int8  KeyReleased : 1;             // �v�nement touche relach�e
   int8  KeyPrevInputValue : 1;       // valeur pr�c�dente de la touche
   int8  KeyValue : 1;                // valeur finale (image) de la touche
 } s_bits;

// structure du descripteur
typedef struct {
           E_DebouceStates DebounceState;    // �tat de l'antirebond
           int8     debounceCounter;         // compteur
           s_bits bits;
} S_SwitchDescriptor;


// constante comptage stabilit� de l'anti-rebond
const int8 MaxDebounceCount = 5;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// POUR CHAQUE SWITCH DONT ON VEUT EFFECTUER L'ANTI-REBOND DANS L'APPLICATION.
// IL FAUDRA DEFINIR UNE VARIABLE DU  TYPE S_SwitchDescriptor
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
// DoDebounce (S_SwitchDescriptor *Descriptor, boolean InputValue)
//              Routine effectuant la gestion de l'anti-rebond d'un switch
//              dont l'adresse de la structure est pass�e en param�tre.
//              La valeur du switch lue sur le port est a passer lors de l'appel
//
// Entr�e: un pointeur sur la structure du switch
//         la valeur du switch lue sur le port
//
// Sortie: la structure mise � jour
//         le bit officiel qui donne la valeur du switch apr�s anti-rebond
//         le bit toggle qui est compl�ment� � chaque pression sur le switch
//         le bit juste press� qui est mis � 1 d�s que l'on presse sur le switch
//           ce bit n'est pas resett� par cette fonction mais doit l'�tre par l'application
//           si elle l'utilise
//         le bit juste rel�ch� qui est mis � 1 d�s que l'on rel�che sur le switch
//           ce bit n'est pas resett� par cette fonction mais doit l'�tre par l'application
//           si elle l'utilise
//

int DoDebounce (S_SwitchDescriptor *Descriptor, int1 InputValue)
{
   int1 PrevInputValue;
   s_bits tmp;
   int return_data = 0;
   
   tmp = Descriptor->bits;
    
   
   // Traitement selon �tat du m�canisme
   switch (Descriptor->DebounceState)
   {
      case DebounceWaitChange :
      PrevInputValue = tmp.KeyPrevInputValue;
         if ( ! (InputValue == PrevInputValue) ) {
               tmp.KeyPrevInputValue = InputValue;
               Descriptor->DebounceState = DebounceWaitStable;
          }
      Break;

      case DebounceWaitStable :
         PrevInputValue = tmp.KeyPrevInputValue;
         if ( InputValue == PrevInputValue )
         {
              Descriptor->DebounceCounter++;
              if (Descriptor->DebounceCounter >= MaxDebounceCount)  {
                  // mise � jour du nouvel �tat
                  return_data=1;
                  tmp.KeyValue = InputValue;
                  if (tmp.KeyValue == 0) {
                     tmp.KeyPressed = true;
                  } else {
                     tmp.KeyReleased = true;
                  }
                  Descriptor->DebounceState = DebounceWaitChange;
                  Descriptor->DebounceCounter = 0;
               }
          } else {
            Descriptor->DebounceCounter = 0;
            tmp.KeyPrevInputValue = InputValue;
         }
      Break;
   }
   Descriptor->bits = tmp;
   
   return(return_data);
 } // end DoDebounce

/********************************************************************************************/
// DebounceInit (S_SwitchDescriptor *pDescriptor) :
//              Routine effectuant l'initialisation de la structure d'un switch
//              dont l'adresse de la structure est pass�e en param�tre.
//
// Entr�e: un pointeur sur la structure du switch
//

void DebounceInit (S_SwitchDescriptor *pDescriptor) {
   pDescriptor->DebounceState     = DebounceWaitChange;   // �tat de l'antirebond
   pDescriptor->debounceCounter   = 0;
   pDescriptor->bits.KeyPressed        = 0;
   pDescriptor->bits.KeyReleased       = 0;
   pDescriptor->bits.KeyPrevInputValue = 1;
   pDescriptor->bits.KeyValue          = 1;
}

//  DebounceGetInput  fourni l'�tat du switch apr�s anti-rebond
int1 DebounceGetInput (S_SwitchDescriptor *pDescriptor) {
   return (pDescriptor->bits.KeyValue);
}

//  DebounceIsPressed    true indique que l'on vient de presser la touche
int1 DebounceIsPressed (S_SwitchDescriptor *pDescriptor) {
   return (pDescriptor->bits.KeyPressed);
}

//  DebounceIsReleased   true indique que l'on vient de relacher la touche
int1 DebounceIsReleased (S_SwitchDescriptor *pDescriptor) {
   return (pDescriptor->bits.KeyReleased);
}

//  DebounceClearPressed    annule indication de pression sur la touche
void DebounceClearPressed  (S_SwitchDescriptor *pDescriptor) {
   pDescriptor->bits.KeyPressed = false;
}

//  DebounceClearReleased   annule indication de relachement de la touche
void DebounceClearReleased  (S_SwitchDescriptor *pDescriptor) {
   pDescriptor->bits.KeyReleased = false;
}
