// SL229_MINF TP SLO2 2013-2014
// Canevas manipulation Menu et générateur signal avec PEC12
// C. HUBER  20/11/2013
// Fichier GenSig.C

// Le mécanisme d'appel cyclique est mis en place
// Il reste à réaliser la gestion des menus simplifié
// ainsi que le générateur de signal


#include "sk18f67j50.h"     // Def PIC & Kit
#include "type_def.h"       // Normalisation des types de base
#include "DefMenuGen.h"     // Def structure des param du générateur

#include "fdebounce.c"          // Gestion anti-rebond
#include "GesPec12.c"           // Gestion du Pec12
#include "SpiDac.c"           // Gestion du Dac
#include "SK18F67J50_driver_lcd.c"    // driver Lcd pour kit 67j50

#include <stdint.h>
#include <math.h>

#include "menu.c"

#include "flash.c"
#include "generator.c"


#define ETOILE '*'
#define ESPACE ' '


S_ParamGen generator;
const uint16 Timer1Reload_1ms = 53536;


uint8 EchNb;      // No de l'échantillon



void main() {


   OSCTUNE_PLLEN = 1;      // Pour obtenir les 48 Mhz
   delay_ms(2);
   
   // Init du lcd
   lcd_init();
   lcd_bl_on();         // allume le rétro-éclairage
   
   // Initialisation du PEC12
   Pec12Init();

   DebounceInit(&DescrESC);   // init debounce touche ESC
   
   // Initialisation SPI et DAC
   InitLTC2604();
   
   Pec12Init();
   // inititialisation du timer1
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   set_timer1(Timer1Reload_1ms);

   // Valeur initiale de la recharge du timer0
   // à  10 Hz periode = 1/(10 * 48) = 2083.33 us
   // à  20 Hz periode = 1/(20 * 48) = 1041.66 us
   // à 990 Hz periode = 1/(990 *48) = 21.043 us 
   // Div 1 OK pour 10 Hz
   // Durée tic = 1/ 12 MHz => 0.083333333 us
   // 2083.33 / 0.083333333 = 25000    OK < 65536
   setup_timer_0(T0_INTERNAL | T0_DIV_1);
   // Calcul recharge pour 20 Hz
   // 1041.66 / 0.083333333 = 12500
   //Timer0Reload = 65536 - 12500;  // valeur de recharge pour 20 Hz
   //set_timer0(Timer0Reload);
   // Control valeur recharge pour 990 Hz
   // 21.043 / 0.083333333 = 252.5  acceptable 
   
   EchNb = 0;
  
   // Affichage du titre
   lcd_putc("\f");
   lcd_gotoxy( 1, 1);
   printf(lcd_putc, "Tp1 GenSig 2013" );
   lcd_gotoxy( 1, 2);
   printf(lcd_putc, "Dolt" );
   lcd_gotoxy( 1, 3);
   printf(lcd_putc, "Palmari" );
   
   delay_ms(3000);       // délai 5 sec
   lcd_clear();
   
   // Autorisation interruption timer 0 et 1
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   
  
   
   
   
   // Initilisation du signal
   generator.Forme = (E_FormesSignal) SignalSinus;
   generator.Frequence = 100;
   generator.Amplitude = 10000;
   generator.Offset = 0;
   generator.Magic = PROGMEM_MAGIC;
   
   progmem_init();
   progmem_load_data(&generator);
   // Initilisation du menu
   menu_init(&generator);
   
   generator_update(&generator);
   
   for(;;) 
   { // boucle sans fin
      delay_ms(10);

      // Gestion des menus
      
      // Test du PEC12
      if(Pec12IsMinus())
      {
         Pec12ClearMinus();
         if(menu_is_locked())
         {
            //update_select_menu(-1);
            menu_add_to_active(-1);
         }
         else
         {
            update_value_menu(-1, &generator);
         }
         
      }
      else if(Pec12IsPlus())
      {
         Pec12ClearPlus();
         if(menu_is_locked())
         {
            menu_add_to_active(+1);
            //update_select_menu(+1);
         }
         else
         {
            update_value_menu(+1, &generator);
         }
         
      }
      else if(Pec12IsOK())
      {
         Pec12ClearOK();
         if(menu_is_locked())
         {
            menu_unset_lock(&generator);
         }
         else
         {
            menu_set_lock(&generator, 0);
            generator_update(&generator);
         }

         //menu.lock = 0;
         
      }
      else if(Pec12IsEsc())
      {
         if(menu_is_locked())
         {
            // Do nothing
         }
         else
         {
            menu_set_lock(&generator,  1);
         }
         //menu.lock = 1;
         Pec12ClearEsc();
      }
      else
      {
      }
      
      if(Pec12NoActivity())
      {
         lcd_bl_off();
      }
      else
      {
         lcd_bl_on();
      }
      //update_select_menu(0);
   if(menu_is_locked())
   {
      if(Pec12Backup())
      {
         Pec12ClearBackup();
         lcd_clear();
         lcd_gotoxy( 1, 1);
         printf(lcd_putc, "Sauvegarde ?");
         
         for(;;)
         {
            if(Pec12IsOK())
            {
               lcd_gotoxy(1,2);
               progmem_save_data(&generator);
               printf(lcd_putc, "Fait !");
               delay_ms(2000);
               break;
            }
            else if(Pec12IsESC())
            {
               break;
            }
            else
            {
               // Do nothing
            }
         }
         lcd_clear();
         menu_init(&generator);
      }
      else
      {
         // Do nothing
      }
   }
   else
   {
    Pec12ClearBackup();
   }
      
   } // end for(;;)
} // end main


#INT_TIMER1
// Réponse à l'interruption du Timer 1
// Cycle = 1 ms
void antiRebond_isr(void)
{
   // recharge Timer1
   set_timer1(Timer1Reload_1ms);

   // traitement cyclique du Pec12
   ScanPec12(input(PEC12_A), input(PEC12_B), input(PEC12_PB), input(S_ESC_MENU));
   
   // Antirebond touche Esc
   DoDebounce (&DescrESc, input(S_ESC_MENU ));  
} // antiRebond_isr

#INT_TIMER0 HIGH
// Réponse à l'interruption du Timer 0
// Cycle échantillonage variable
void sampling_isr(void)
{
   output_high(LIGNE3);    // marque début
   
   // recharge Timer0
   set_timer0(Timer0Reload);

   // traitement d'un échantillon
   // ---------------------------
   
   // Test du dac (a remplacer par la suite)
   // 1394 * 47 = 65518 poroche du max (65535)
   //WriteDac(1394 * EchNb, 0);      // sur canal 0
   WriteDac(GENDATA[EchNb], 0);      // sur canal 0
   EchNb++;
   EchNb = EchNb % 48;
   
   output_low(LIGNE3);    // marque fin    
} // sampling_isr

// Fontions de traitement des éléments de menu

