// GestRS232.C
// base module gestion communication RS232 pout Tp1B_GenSigRs232 SLO2 2013-2014
// Utilisation directe des registres SFR
// #use RS232 pour init
// Adaptation pour 18F67j50
// C. HUBER 02.05.2012
// Baud rate 57600-N-1

// Ce fichier contient :


// L'émission est réalisée par l'INT_TBE2
// La réception des caractères est réalisée par INT_RDA2

// La reception des message est traité dans l'int à 10 ms
// Gestion du control de flux hardware
// L'émission des message est traité dans l'int à 20 ms
// Gestion du control de flux hardware

// Les fonctions de conversion pour le ASCII HEX <-> numérique sont fournies




#include "GesFifoTh.c"
 
// prototypes des fonctions
void InitGestRs232(void);
int GetMessage(S_ParamGen * data);
void SendMessage(void);
int CheckMessage(char * mess);
void DecodeMessage(char * mess, S_ParamGen * data);


// Definition pour les messages


// Structure décrivant le message
typedef struct {
   char Start;
   char Forme;
   char Frequence[3];
   char Amplitude[5];
   char Offset[5];
} StruMess;

#define MESS_SIZE  (sizeof(StruMEss))

// union pour composition et emission/réception des messages
typedef union {
   StruMess Trame;
   char Mess[MESS_SIZE];
} uMess;



// union pour émission des messages
uMess TxMess;
// union pour réception des messages
uMess RxMess;

// Pour réception
#define  WaitSTX        0
#define  WaitSuite      1

// registres USART2, usage minimal
#byte TXREG2 = 0xFA9 // serial transmit register 2 
#byte RCREG2 = 0xFAA // serial receive register 2
#byte RCSTA2 = 0xF9C // serial receive status 2

#byte PIR3  = 0xfA4 // peripheral interrupt request 3

#define TXIF_mask 0x10  // masque test TXIF2 bit de PIR3
#define RCIF_mask 0x20  // masque test RCIF2 bit de PIR3
#define RCERR_mask 0x06  // masque test erreurs dans RCSTA2

// Gestion des erreur RX
// FERR: Framing Error bit
// 1 = Framing error (can be updated by reading RCREG register and receiving next valid byte)
// 0 = No framing error
// bit 1 OERR: Overrun Error bit
// 1 = Overrun error (can be cleared by clearing bit CREN)
// 0 = No overrun error

// Declaration des FIFO pour réception et émission
const sint8 FIFO_RX_SIZE = ((3* MESS_SIZE) + 1);  // 3 mess + 1 char
const sint8 FIFO_TX_SIZE = ((3* MESS_SIZE) + 1);  // 3 mess + 1 char



sint8 fifoRX[FIFO_RX_SIZE];
// Declaration du descripteur du FIFO de réception
S_fifo descrFifoRX;

sint8 fifoTX[FIFO_TX_SIZE];
// Declaration du descripteur du FIFO d'émission
S_fifo descrFifoTX;


// Réponse Interruption de réception
// un interrupt arrive lorsqu'un byte vient d'être
// transféré dans le buffer de réception

// Fiforisation et gestion du control de flux
// Le RTS doit être remis à low hors interruption !

#int_RDA2 //HIGH
void RDA2_isr(void)
{
   uint8 ErrFiFoFull = 0;
   uint8 freeSize;

   // Test si erreur comm
   if ( (RCSTA2 & RCERR_mask) == 0) {
      // dépot inconditionnel dans le fifo
      PutCharInFifo ( &descrFifoRX, RCREG2);
   } else {
      bit_clear(RCSTA2, 4);     // CREN = 0
      freeSize = RCREG2;        // purge
      bit_set(RCSTA2, 4);       // CREN = 1
    }
   
   freeSize = GetWriteSpace ( &descrFifoRX);
   if (freeSize <= 2 )
   {
      // Demande de ne plus émettre 
      output_high(RS232_RTS);
      if (freeSize == 0) {
            ErrFiFoFull = 1;    // pour debugging si probème ctrl flux
      }
   }
} // RDA_isr



#INT_TBE2
void TBE2_isr(void)
{
   int8 c;
   int8 size;
   int1 i_cts;
   
   size = GetReadSize (&descrFifoTX);
   i_cts = input(RS232_CTS); 
   // On vérifie 2 conditions :
   //    Si CTS = 0 (autorisation d'émettre)
   //    Si il y a un caratères à émettre
   if ( ( i_cts == 0) && ( size > 0 )  ) {

     GetCharFromFifo(&descrFifoTX, &c);
     TXREG2 = c;  // émet 1 char
   } else {
      disable_interrupts(INT_TBE2);
   }
}

void InitGestRs232(void) {

   char scratch;
   
      
   // Initialisation du fifo de réception
   InitFifo ( &descrFifoRX, FIFO_RX_SIZE, fifoRX, 0 );
   // Initialisation du fifo d'émission
   InitFifo ( &descrFifoTX, FIFO_TX_SIZE, fifoTX, 0 );

  
   // efface les interruptions qui pourraient rester et lit un éventuel byte
   // dans le buffer de réception
   scratch = RCREG2;
   output_low(RS232_RTS);  // autorise emission host

 
} // end InitGestRs232


int GetMessage(S_ParamGen * data)
{
      uint8_t quit = 0;
      uint8_t i = 0;

      char mess[15]; 
      
      while( quit != 1)
      {
         lcd_gotoxy( 2, 1);
         if(GetReadSize(&descrFifoRX) >= 15)
         {
            
            GetCharFromFifo ( &descrFifoRX, &mess[0] );
            
            //if((uint8_t) mess[0] == 33)
            if(mess[0] == '!')
            {         
               // Lecture du messuge
               for(i = 1 ; i < 15 ; i++)
               {
                  GetCharFromFifo ( &descrFifoRX, &mess[i] ); 
               }
               quit = 1;
               
            }
            else
            {
               
               // Do nothing
            }
         }
         else
         {
            return 1;
         }
      }
      
      
      // Gestion control de flux de la réception
      if(GetWriteSpace ( &descrFifoRX) >= 4) {
           output_low(RS232_RTS);  // autorise émission par l'autre
      }
      
      if(CheckMessage(mess) == 0)
      {
         DecodeMessage(mess, data);
         return 0;
      }
      else
      {
         return 1;
      }
} // GetMessage


// Fonction d'envoi des message, appel cyclique
// Envoi 1 message à chaque appel (si assez de place dans Fifo)


void SendMessage(S_ParamGen * data)
{
   sint8 freeSize;
   int1 ValCTS;
   char tx_char[15];
   int i;
    
   // Préparation du message et dépôt dans le fifo 
   tx_char[0] = '!';
   switch(data->Forme)
   {
      case 0:
         tx_char[1] = 'C';
         break;
      case 1:
         tx_char[1] = 'T';
         break;
      case 2:
         tx_char[1] = 'S';
         break;
      case 3:
         tx_char[1] = 'D';
         break;
      default:
         tx_char[1] = 'E';
         break;
   }
   
   // Encode Frequence:
   tx_char[2] = (data->Frequence /100) + '0';
   tx_char[3] = ((data->Frequence%100) /10) + '0';
   tx_char[4] = (data->Frequence%10) + '0';
   
   
   // Encode Amplitude
   tx_char[5] = (data->Amplitude /10000) + '0';
   tx_char[6] = ((data->Amplitude%10000) /1000) + '0';
   tx_char[7] = ((data->Amplitude%1000) /100) + '0';
   tx_char[8] = ((data->Amplitude%100) /10) + '0';
   tx_char[9] = (data->Amplitude%10) + '0';
   
   
   if(data->Offset < 0)
   {
      tx_char[10] = '-';
   }
   else
   {
      tx_char[10] = '+';
   }
   
    // Décode Offset
   tx_char[11] = ((data->Offset%10000) /1000) + '0';
   tx_char[12] = ((data->Offset%1000) /100) + '0';
   tx_char[13] = ((data->Offset%100) /10) + '0';
   tx_char[14] = (data->Offset%10) + '0';

    
   if(GetWriteSpace(&descrFifoTX)>=15)
   {
      for(i = 0; i <15; i++)
      {
         PutCharInFifo(&descrFifoTX,tx_char[i]);
      }
   }
   // Gestion du control de flux
   // si on a un caractère à envoyer et que CTS = 0
   freeSize = GetReadSize(&descrFifoTX);
   ValCTS = input(RS232_CTS);
   if ((ValCTS == 0) && (freeSize > 0))
   {
         // Autorise int émission
         enable_interrupts(INT_TBE2);
   }
}


int CheckMessage(char * mess)
{
   uint8_t check_error = 0;
   uint8_t i = 0;
   
   if(mess[0] != '!')
   {
      check_error = 1;
   }
   
   if((  mess[1] == 'C' || mess[1] == 'S' || mess[1] == 'T' || mess[1] == 'D'))
   {
      // Do nothing
   }
   else
   {
      check_error = 1;
   }
   
   // Check fréqunce
   for( i = 0 ; i < 3 ; i++)
   {
      if( (mess[i+2] < '0') || (mess[i+2] > '9') )
      {
         check_error = 1;
      }
   }
   
   // Check Amplitude
   for( i = 0 ; i < 5 ; i++)
   {
      if( (mess[i+5] < '0') || (mess[i+5] > '9') )
      {
         check_error = 1;
      }
   }
   
   
   // Check Offset
   if( mess[10] == '+' || mess[10] == '-')
   {
   }
   else
   {
         check_error = 1;
   }
   
   for( i = 0 ; i < 4 ; i++)
   {
      if( (mess[i+11] < '0') || (mess[i+11] > '9') )
      {
         check_error = 1;
      }
   }
   
   return check_error;
}


void DecodeMessage(char * mess, S_ParamGen * data)
{
   // Decode FORME:
   switch(mess[1])
   {
      case 'C':
         data->Forme = SignalCarre;
         break;
      case 'S':
         data->Forme = SignalSinus;
         break;
      case 'T':
         data->Forme = SignalTriangle;
         break;
      case 'D':
         data->Forme = SignalDentDeScie;
         break;
      default:
         break;
   }
   
   // Decode Frequence:
   data->Frequence =  ( (uint16_t) mess[2] - '0' ) * 100;
   data->Frequence += ( (uint16_t) mess[3] - '0' ) * 10;
   data->Frequence += ( (uint16_t) mess[4] - '0' );
   
   // Decode Amplitude
   data->Amplitude =  ( (uint16_t) mess[5] - '0' ) * 10000;
   data->Amplitude += ( (uint16_t) mess[6] - '0' ) * 1000;
   data->Amplitude += ( (uint16_t) mess[7] - '0' ) * 100;
   data->Amplitude += ( (uint16_t) mess[8] - '0' ) * 10;
   data->Amplitude += ( (uint16_t) mess[9] - '0' );
   
   // Décode Offset
   data->Offset =  ( (uint16_t) mess[11] - '0' ) * 1000;
   data->Offset += ( (uint16_t) mess[12] - '0' ) * 100;
   data->Offset += ( (uint16_t) mess[13] - '0' ) * 10;
   data->Offset += ( (uint16_t) mess[14] - '0' ) ;
   
   if (mess[10] == '-') 
   {
     data->Offset *= (-1);
   }

   
   
}
