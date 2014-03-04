// Définitions pour Starter Kit avec 18f4520
#include <18F4520.h>
#device adc=10
// fuses pour programmation définitive
// 18f4520 NOOSCSEN n'est plus valide
#fuses H4, NOPROTECT, BROWNOUT, NOWDT, BORV27, PUT, CCP2C1, STVREN, NODEBUG, NOLVP, NOWRT, NOWRTD, NOWRTB, NOWRTC, NOCPD, NOCPB, NOEBTR, NOEBTRB
#use delay (clock=40000000)  // 40MHz clock interne


// Re définition des entrées-sorties
#define Analog1   PIN_A0
#define Analog2   PIN_A1
#define LIGNE1    PIN_A2
#define LIGNE2    PIN_A3
#define LIGNE3    PIN_A4
#define LIGNE4    PIN_A5

#define S_ESC_MENU   PIN_B0
#define S_PLUS       PIN_B1
#define S_MINUS      PIN_B2
#define S_OK         PIN_B3
#define COLONNE1     PIN_B4
#define COLONNE2     PIN_B5
#define COLONNE3     PIN_B6
#define COLONNE4     PIN_B7

// Ajout pour PEC12
#define PEC12_A      PIN_B1
#define PEC12_B      PIN_B2
#define PEC12_PB     PIN_B3


#define I2C_SDA      PIN_C0
#define I2C_SCL      PIN_C1
#define CS_LM70      PIN_C2
#define SPI_SCL      PIN_C3
#define SPI_SDI      PIN_C4
#define SPI_SDO      PIN_C5
#define RS232_TX     PIN_C6
#define RS232_RX     PIN_C7

#define LCD_RS       PIN_D0
#define LCD_RW       PIN_D1
#define LCD_E        PIN_D2
#define LCD_BL       PIN_D3
#define LCD_DB4      PIN_D4
#define LCD_DB5      PIN_D5
#define LCD_DB6      PIN_D6
#define LCD_DB7      PIN_D7

#define RTS       PIN_E0
#define CTS       PIN_E1
#define LED_D1    PIN_E2
