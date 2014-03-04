   /*******************************************************************************
   ********************************************************************************
   *****                                                                      *****
   *****                    DEFINITION DES PINS DU SK18F67J50                 *****
   *****                                                                      *****
   ********************************************************************************
   *****                                                                      *****
   *****                                                                      *****
   *****       NOM DU FICHIER :  SK18F67J50.h                                 *****
   *****                                                                      *****
   *****       DESCRIPTION:                                                   *****
   *****                     -   Définition des pins                          *****
   *****                                                                      *****
   *****                     -   Définition des fusibles                      *****
   *****                                                                      *****
   *****                                                                      *****
   *****                                                                      *****
   *****                                                                      *****
   *****     Chevalley Gael                                                   *****
   *****     C. HUBER 15.03.2011  Adaptation Labo SLO1                        *****
   *****     ETML-ES                                                          *****
   *****                                                                      *****
   *****                                                                      *****
   ********************************************************************************
   *******************************************************************************/
   
   
#include <18F67J50.h>

#device adc=10
#device HIGH_INTS=TRUE
// fuses pour programmation définitive
// Quartz 20 MHz, config pour 48 MHz


// Configuration quartz 20 Mhz  clock USB 48 MHz, clock CPU 48 Mhz
#fuses  HSPLL,  PLL5, NOCPUDIV, NOWDT, STVREN, NOXINST, NODEBUG, NOPROTECT,  NOFCMEN, NOIESO, CCP2C1
#use delay (clock=48000000)  // 48 MHz system Clock
// RS232 USART 2
#use rs232(UART2,baud=9600,parity=N,xmit=PIN_G1,rcv=PIN_G2,bits=8, stop=1)


// Ajout OSCTUNE
#byte OSCTUNE = 0xF9B                  // corrigé de F60 en F9B
#bit OSCTUNE_PLLEN = OSCTUNE.6


/*************************************
**************************************
**  Définition des entrées-sorties  **
**************************************
*************************************/

//Analogique
#define Analog1      PIN_A0
#define Analog2      PIN_A1


//Lignes & Ethernet
#define EthCS/L1     PIN_B0
#define EthInt/L2    PIN_B1
#define LIGNE3       PIN_B2
#define LIGNE4       PIN_B3


//Colonnes
#define COLONNE1     PIN_A2
#define COLONNE2     PIN_A3
#define COLONNE3     PIN_A4
#define COLONNE4     PIN_A5


//PEC12 + Touches
#define S_OK         PIN_F2
#define S_ESC_MENU   PIN_F5
#define S_PLUS       PIN_F6
#define S_MINUS      PIN_F7

#define PEC12_A      S_PLUS
#define PEC12_B      S_MINUS
#define PEC12_PB     S_OK 

//I2C
#define I2C_SCL      PIN_C3
#define I2C_SDA      PIN_C4


//SPI
#define SPI_SCL      PIN_D6
#define SPI_SDI      PIN_D5
#define SPI_SDO      PIN_D4
#define CS_LM70      PIN_D0
#define CS_DAC       PIN_D1
#define CS_SD        PIN_D2
#define CS_RTC       PIN_D3

// LEDs (en commun avec CS)
#define LED_D5       PIN_D0
#define LED_D6       PIN_D1
#define LED_D7       PIN_D2
#define LED_D8       PIN_D3

//RS232
#define RS232_TX     PIN_G1
#define RS232_RX     PIN_G2
#define RS232_RTS    PIN_G3
#define RS232_CTS    PIN_G4


//Gestion LCD
#define LCD_RS       PIN_E0
#define LCD_RW       PIN_E1
#define LCD_E        PIN_E2
#define LCD_BL       PIN_E3
#define LCD_DB4      PIN_E4
#define LCD_DB5      PIN_E5
#define LCD_DB6      PIN_E6
#define LCD_DB7      PIN_E7


//Pont en H
#define DirH         PIN_B4
#define BrakeH       PIN_B5
#define PWMH         PIN_C2


//XBee
#define XBEE_TX      PIN_C6
#define XBEE_RX      PIN_C7
#define XBEE_RTS     PIN_C0
#define XBEE_CTS     PIN_C1


//Divers
#define SD_Detect    PIN_C5
#define ResetCarte   PIN_D7
