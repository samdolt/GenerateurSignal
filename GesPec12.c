// GesPec12.c  Canevas pour réalisation travail pratique 
// C. HUBER    23/08/2010

// Fonctions pour la gestion du Pec12
//
//
// Principe : Il est nécessaire d'appeler cycliquement la fonction ScanPec12
//            avec un cycle de 1 ms
//
//  Pour la gestion du Pec12, il y a 10 fonctions à disposition :
//  Initialisation
//       Pec12Init
//  Fonctions de gestion

//       Pec12IsPlus       true indique un nouveau incrément
//       Pec12IsMinus      true indique un nouveau décrément
//       Pec12IsOK         true indique action OK
//       Pec12IsESC        true indique action ESC
//       Pec12NoActivity   true indique abscence d'activité sur PEC12
//  Fonctions pour quittance des indications
//       Pec12ClearPlus    annule indication d'incrément
//       Pec12ClearMinus   annule indication de décrément
//       Pec12ClearOK      annule indication action OK
//       Pec12ClearESC     annule indication action ESC
//
//
//---------------------------------------------------------------------------

// Attention dans le module
// définitions des types qui seront utilisés dans cette application



// structure du descripteur du PEC12
typedef struct {
            bool Inc;             // événement incrément  
            bool Dec;             // événement décrément 
            bool OK;              // événement action OK
            bool ESC;             // événement action ESC
            bool NoActivity;      // Indication de non activité
            uint16 PressDuration;        // Pour durée pression du P.B.
            uint16 InactivityDuration;   // Pour durée inactivité
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
// Aprés l'appel le descripteur est mis à jour
//         
//


void ScanPec12 (bool ValA, bool ValB, bool ValPB)
{
   // Traitement antirebond sur A, B PB
   DoDebounce (&DescrA, ValA);
   DoDebounce (&DescrB, ValB);
   DoDebounce (&DescrPB, ValPB);
   
   // Détection incrément / décrément
   
   // Test si flanc montant sur A
   If ( DebounceIsReleased (&DescrA) )  {
      // Quittance
      DebounceClearReleased(&DescrA);
      if (DebounceGetInput(&DescrB) == 0 ) {
         // sens horaire = incrément
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
   Pec12.Inc = 0;             // événement incrément  
   Pec12.Dec = 0;             // événement décrément 
   Pec12.OK = 0;              // événement action OK
   Pec12.ESC = 0;             // événement action ESC
   Pec12.NoActivity = 1;      // Indication de non activité
   Pec12.PressDuration = 0;   // Pour durée pression du P.B.
   Pec12.InactivityDuration = 0; // Durée inactivité
}




//       Pec12IsPlus       true indique un nouveau incrément
bool Pec12IsPlus    (void) {
   return (Pec12.Inc);
}

//       Pec12IsMinus      true indique un nouveau décrément
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

//       Pec12NoActivity   true indique abscence d'activité sur PEC12
bool Pec12NoActivity    (void) {
   return (Pec12.NoActivity);
}

//  Fonctions pour quittance des indications
//  ----------------------------------------

//       Pec12ClearPlus    annule indication d'incrément
void Pec12ClearPlus   (void) {
   Pec12.Inc = 0;
}

//       Pec12ClearMinus   annule indication de décrément
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

