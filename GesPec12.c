// GesPec12.c  Canevas pour r�alisation travail pratique 
// C. HUBER    23/08/2010

// Fonctions pour la gestion du Pec12
//
//
// Principe : Il est n�cessaire d'appeler cycliquement la fonction ScanPec12
//            avec un cycle de 1 ms
//
//  Pour la gestion du Pec12, il y a 10 fonctions � disposition :
//  Initialisation
//       Pec12Init
//  Fonctions de gestion

//       Pec12IsPlus       true indique un nouveau incr�ment
//       Pec12IsMinus      true indique un nouveau d�cr�ment
//       Pec12IsOK         true indique action OK
//       Pec12IsESC        true indique action ESC
//       Pec12NoActivity   true indique abscence d'activit� sur PEC12
//  Fonctions pour quittance des indications
//       Pec12ClearPlus    annule indication d'incr�ment
//       Pec12ClearMinus   annule indication de d�cr�ment
//       Pec12ClearOK      annule indication action OK
//       Pec12ClearESC     annule indication action ESC
//
//
//---------------------------------------------------------------------------

// Attention dans le module
// d�finitions des types qui seront utilis�s dans cette application



// structure du descripteur du PEC12
typedef struct {
            bool Inc;             // �v�nement incr�ment  
            bool Dec;             // �v�nement d�cr�ment 
            bool OK;              // �v�nement action OK
            bool ESC;             // �v�nement action ESC
            bool NoActivity;      // Indication de non activit�
            uint16 PressDuration;        // Pour dur�e pression du P.B.
            uint16 InactivityDuration;   // Pour dur�e inactivit�
} S_Pec12_Descriptor;


// Structure pour les traitement du Pec12
S_Pec12_Descriptor Pec12;


// Descripteur des sinaux du Pec12
S_SwitchDescriptor DescrA;
S_SwitchDescriptor DescrB;
S_SwitchDescriptor DescrPB;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Principe utilisation des fonctions
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
// ScanPec12 (bool ValA, bool ValB, bool ValPB)
//              Routine effectuant la gestion du Pec12
//              recoit la valeur des signaux et du boutons
//
// s'appuie sur le descripteur global.
// Apr�s l'appel le descripteur est mis � jour
//         
//


void ScanPec12 (bool ValA, bool ValB, bool ValPB)
{
   // Traitement antirebond sur A, B PB
   DoDebounce (&DescrA, ValA);
   DoDebounce (&DescrB, ValB);
   DoDebounce (&DescrPB, ValPB);
   
   // D�tection incr�ment / d�cr�ment
   
   // Test si flanc montant sur A
   If ( DebounceIsReleased (&DescrA) )  {
      // Quittance
      DebounceClearReleased(&DescrA);
      if (DebounceGetInput(&DescrB) == 0 ) {
         // sens horaire = incr�ment
         Pec12.Inc = 1;
      } else {
         Pec12.Dec = 1;
      }
   
   } 
   
   
   // Traitement du PushButton
   
 } // ScanPec12



//  Pec12Init
void Pec12Init (void) {
   // Initialisation des descripteurs de touches Pec12
   DebounceInit(&DescrA);
   DebounceInit(&DescrB);
   DebounceInit(&DescrPB);
   
   // Init de la structure PEC12
   Pec12.Inc = 0;             // �v�nement incr�ment  
   Pec12.Dec = 0;             // �v�nement d�cr�ment 
   Pec12.OK = 0;              // �v�nement action OK
   Pec12.ESC = 0;             // �v�nement action ESC
   Pec12.NoActivity = 1;      // Indication de non activit�
   Pec12.PressDuration = 0;   // Pour dur�e pression du P.B.
   Pec12.InactivityDuration = 0; // Dur�e inactivit�
}




//       Pec12IsPlus       true indique un nouveau incr�ment
bool Pec12IsPlus    (void) {
   return (Pec12.Inc);
}

//       Pec12IsMinus      true indique un nouveau d�cr�ment
bool Pec12IsMinus    (void) {
   return (Pec12.Dec);
}

//       Pec12IsOK         true indique action OK
bool Pec12IsOK    (void) {
   return (Pec12.OK);
}

//       Pec12IsESC        true indique action ESC
bool Pec12IsESC    (void) {
   return (Pec12.ESC);
}

//       Pec12NoActivity   true indique abscence d'activit� sur PEC12
bool Pec12NoActivity    (void) {
   return (Pec12.NoActivity);
}

//  Fonctions pour quittance des indications
//  ----------------------------------------

//       Pec12ClearPlus    annule indication d'incr�ment
void Pec12ClearPlus   (void) {
   Pec12.Inc = 0;
}

//       Pec12ClearMinus   annule indication de d�cr�ment
void Pec12ClearMinus   (void) {
   Pec12.Dec = 0;
}

//       Pec12ClearOK      annule indication action OK
void Pec12ClearOK   (void) {
   Pec12.OK = 0;
}

//       Pec12ClearESC     annule indication action ESC
void Pec12ClearEsc   (void) {
   Pec12.ESC = 0;
}

