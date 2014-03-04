// SL229_MINF TP SLO2 2013-2014
// TP1A : Générateur de signal avec contrôle local
// Samuel Dolt - Claudio Palmari
// Fichier GenSig.C

#include "sk18f67j50.h"     // Def PIC & Kit
#include "type_def.h"       // Normalisation des types de base
#include "DefMenuGen.h"     // Def structure des param du générateur

#include "fdebounce.c"          // Gestion anti-rebond
#include "GesPec12.c"           // Gestion du Pec12
#include "SpiDac.c"           // Gestion du Dac
#include "SK18F67J50_driver_lcd.c"    // driver Lcd pour kit 67j50

#include <stdint.h>
#include <math.h>
#include <string.h>

#include "menu.c"

#include "flash.c"
#include "generator.c"

#include "GestRS232.c"


#define ETOILE '*'
#define ESPACE ' '


S_ParamGen generator;
S_ParamGen RemoteGenerator;
S_ParamGen OldGeneratorValue;
S_ParamGen SendGenerator;

const uint16 Timer1Reload_1ms = 53536;


uint8 EchNb;      // No de l'échantillon



void main() {

   int cycle_counter = 0;
   int old_state = 1;
   int connect_counter = 0;
   int local_flag = 1;
   int read_cnt_value = 1;

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
   
   // inititialisation du timer1
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   set_timer1(Timer1Reload_1ms);

   // Valeur initiale Ide la recharge du timer0
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
  
   // Affichage du titre
   lcd_putc("\f");
   lcd_gotoxy( 1, 1);
   printf(lcd_putc, "Tp1B GenSig 2013" );
   lcd_gotoxy( 1, 2);
   printf(lcd_putc, "Dolt" );
   lcd_gotoxy( 1, 3);
   printf(lcd_putc, "Palmari" );
   
   delay_ms(5000);       // délai 5 sec
   lcd_clear();
   
   // Autorisation interruption timer 0 et 1Touc
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RDA2);
   enable_interrupts(GLOBAL);
   
   
   // Initilisation du signal
   generator.Forme = (E_FormesSignal) SignalSinus;
   generator.Frequence = 100;
   generator.Amplitude = 10000;
   generator.Offset = 0;
   generator.Magic = PROGMEM_MAGIC;
   RemoteGenerator.Forme = (E_FormesSignal) SignalSinus;
   RemoteGenerator.Frequence = 100;
   RemoteGenerator.Amplitude = 10000;
   RemoteGenerator.Offset = 0;
   RemoteGenerator.Magic = PROGMEM_MAGIC;
 
   OldGeneratorValue = RemoteGenerator;
   SendGenerator = generator;

 
   progmem_init();
   progmem_load_data(&generator);
   // Initilisation du menu
   menu_init(&generator);
   
   generator_update(&RemoteGenerator);
   
   InitGestRs232();

   
   for(;;) 
   { // boucle sans fin
    delay_ms(10);

    // Traitement de l'apuis sur les touches
    if(Pec12IsMinus())
    {
        Pec12ClearMinus();
        if(menu_is_locked())
        {
            // Sélectionne la ligne supérieur du menu
            menu_add_to_active(-1);
        }
        else
        {
            // Décrémente d'un saut la valeur de la ligne sélectionnée
            update_value_menu(-1, &generator);
        }
         
    }
    else if(Pec12IsPlus())
    {
        Pec12ClearPlus();
        if(menu_is_locked())
        {
            // Sélectionne la ligne inférieur du menu
            menu_add_to_active(+1);
        }
        else
        {
            // Incrémente d'un saut la valeur de la ligne sélectionnée
            update_value_menu(+1, &generator);
        }
         
    }
    else if(Pec12IsOK())
    {
        Pec12ClearOK();
        if(menu_is_locked())
        {
            // Dévérouille le menu pour permettre l'édition
            menu_unset_lock(&generator);
        }
        else
        {
            // Vérouille le menu et met à jour la table des échantillons
            menu_set_lock(&generator, 0);

            
            if(local_flag == 0)
            {
               SendGenerator = generator;
            }
            else
            {
               generator_update(&generator);
            }
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
    }
    else
    {
        // Si aucune touche n'est préssée, ne rien faire
    }
      
	// Traitement de l'inactivité
	// Cette partie permet de commuter en mode local quand la communication échoue
    if(Pec12NoActivity())
    {
        if(local_flag == 0)
        {
            menu_init_info_char('>','>','>','>');
            menu_init_value(&RemoteGenerator);
        }
        else
        {

            menu_init_info_char('|','|','|','|');
            menu_init_value(&generator);
        }
    }
    else
    {
        menu_init_value(&generator);
        menu_init_info_char(' ',' ',' ',' ');
        menu_goto(menu_get_active());
    }
      
      
    // Traitement de la sauvegarde
	if(menu_is_locked())
	{
		// Si l'événement sauvegarde est reporté
		if(Pec12Backup())
		{
			// Affichage du message de sauvegarde
			Pec12ClearBackup();
			lcd_clear();
			lcd_gotoxy( 1, 1);
			printf(lcd_putc, "Sauvegarde ?");

         
            // Boucle infinie pour l'attente de l'appuis sur la touche OK ou ESC
            for(;;)
            {
                if(Pec12IsOK())
                {
                    // Appuis sur la touche OK
                    // Sauvegarde des paramètres en mémoire programme
                    // Affichage du message "Fait" pendant deux secondes
                    progmem_save_data(&generator);
                    lcd_gotoxy(1,2);
                    printf(lcd_putc, "Fait !");
                    delay_ms(2000);
                    break;
                }
				else if(Pec12IsESC())
				{
					// Appuis sur la touche ESC
					// Anulation de la souvegarde
					break;
				}
				else
				{
					// Pas de touche appuyée, ne rien faire
				}
			}
			// Remise à zéro de l'écran et affichage des valeurs
			lcd_clear();
			menu_init(&generator);
		}
		else
		{
			// Si l'événement sauvegarde n'a pas été reporté, ne rien faire
		}
	}
	else
	{
		Pec12ClearBackup();
		// Si le menu est vérouiller, on efface l'éventuelle évenement déclenchant une sauvegarde
	}
   
	// FUNCTION RX
	// generator_update(&generator);
	//GetMessage(&RemoteGenerator);
   

   
	if(GetMessage(&RemoteGenerator) == 0)
	{
		generator_update(&RemoteGenerator);
		connect_counter = 0;
		local_flag = 0;
	}
	else
	{
		// Pas de message recu
		if(connect_counter == 50)
		{
            //SendGenerator
            local_flag = 1;
            generator_update(&generator);
		}
		else if(connect_counter > 50)
		{
			// Do nothing
		}
		else
		{
			connect_counter++;
		}

	}


	if(cycle_counter==1)
	{
		// FUNCTION TX
		SendMessage(&SendGenerator);
		cycle_counter=0;
	}
	else
	{
		cycle_counter=1;
	}
    
    } // Fin de la boucle infinie principale
} // Fin de la fonction main



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
