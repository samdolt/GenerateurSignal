// menu.c - Samuel Dolt
// Ce fichier implémente le menu du générateur de signal 

// Marque utilisée pour sélectionner une entrée, lorsque le menu est vérouillé
#define LOCK_MARK '*'

// Marque utilisée pour représenter la ligne du menu que l'on peut modifier 
// lorsque le menu est dévérouillé
#define UNLOCK_MARK '>'

#define MENU_FORME 0
#define MENU_FREQ 1
#define MENU_AMPL 2
#define MENU_OFFSET 3

// Texte représentant les formes possibles de signal à générer
const char MENU_VALUE[4][6] = {"CARRE","TRIAN","SINUS","DDS"};

// Incrément, valeur minimale et maximal pour la sélection de la forme
const uint16_t FORME_INCR = 1;
const uint16_t FORME_MIN = 0;
const uint16_t FORME_MAX = 3;

// Incrément, valeur minimale et maximal pour la sélection de la fréquence
const uint16_t FREQUENCY_INCR = 10;
const uint16_t FREQUENCY_MIN = 10;
const uint16_t FREQUENCY_MAX = 990;

// Incrément, valeur minimale et maximal pour la sélection de l'amplitude
const uint16_t AMPLITUDE_INCR = 50;
const uint16_t AMPLITUDE_MIN = 0;
const uint16_t AMPLITUDE_MAX = 10000;

// Incrément, valeur minimale et maximal pour la sélection de l'offset
const uint16_t OFFSET_INCR = 50;
const int16_t OFFSET_MIN = -5000;
const int16_t OFFSET_MAX = 5000;

// Structure servant de descripteur au menu
struct menu_s
{
   int8_t active;
   int8_t lock;
   char mark;
   uint16_t old;
}_menu;

// Retourne 1 si le menu est vérouillé
// Retourne 0 si le menu est dévérouillé
static inline bool menu_is_locked(void)
{
   return _menu.lock;
}

// Met à jour la marque sur la ligne sélectionnée.
void update_mark(void)
{
   switch(_menu.active)
   {
      case MENU_FORME:
         lcd_gotoxy( 1, 1);
         break;
      case MENU_FREQ:
         lcd_gotoxy( 1, 2);
         break;
      case MENU_AMPL:
         lcd_gotoxy( 1, 3);
         break;
      case MENU_OFFSET:
         lcd_gotoxy( 1, 4);
         break;
      default:
         break;
   }
   lcd_putc(_menu.mark);
}

// Retourne la marque utilisée sur la ligne sélectionnée
static inline char menu_get_mark(void)
{
   return _menu.mark;
}

// Permet de sélectionner et mettre à jour la marque utilisée pour sélectionner une ligne
static inline void menu_set_mark(char mark)
{
   _menu.mark = mark;
   update_mark();
}

// Dévérouille le menu
static inline char menu_get_active(void)
{
   return _menu.active;
}

void menu_unset_lock(S_ParamGen * data)
{
   _menu.lock = 0;
   menu_set_mark(UNLOCK_MARK);
   switch(_menu.active)
      {
         case MENU_FORME:
            _menu.old = (uint16_t) data->Forme;
            break;
         case MENU_FREQ:
            _menu.old = (uint16_t) data->Frequence;
            break;
         case MENU_AMPL:
            _menu.old = (uint16_t) data->Amplitude;
            break;
         case MENU_OFFSET:
            _menu.old = (uint16_t) data->Offset;
            break;
         default:
            break;
      } 
}

// Sélectionne une ligne
void menu_goto(int8_t x)
{
   // Effacement de menu en cours
   switch(_menu.active)
   {
      case MENU_FORME:
         lcd_gotoxy( 1, 1);     
         break;
      case MENU_FREQ:
         lcd_gotoxy( 1, 2);
         break;
      case MENU_AMPL:
         lcd_gotoxy( 1, 3);
         break;
      case MENU_OFFSET:
         lcd_gotoxy( 1, 4);
         break;
      default:
         break;
   }
   lcd_putc(' ');
            
   // Check ifx is valid and go to it   
   if(x < MENU_FORME)
   {
      _menu.active = MENU_OFFSET;
   }
   else if(x > MENU_OFFSET)
   {
      _menu.active = MENU_FORME;
   }
   else
   {
      // Go to menu x
      _menu.active = x;
   }
   
   // Print symbol befor selected menu
   switch(_menu.active)
   {
      case MENU_FORME:
         lcd_gotoxy( 1, 1);
         break;
      case MENU_FREQ:
         lcd_gotoxy( 1, 2);
         break;
      case MENU_AMPL:
         lcd_gotoxy( 1, 3);
         break;
      case MENU_OFFSET:
         lcd_gotoxy( 1, 4);
         break;
      default:
         break;
   }
   lcd_putc(_menu.mark);
}

// Met à jour la valeur affichée dans le menu en rajoutant un incrément
void update_value_menu(int8_t incr, S_ParamGen * data )
{
   // Effacement de la valeur en cours
   switch(_menu.active)
        {
            case MENU_FORME:
				lcd_gotoxy( 16, 1);
				break;
            case MENU_FREQ:
				lcd_gotoxy( 16, 2);
				break;
            case MENU_AMPL:
				lcd_gotoxy( 16, 3);
				break;
            case MENU_OFFSET:
				lcd_gotoxy( 16, 4);   
				break;
            default:
				break;
         }
         printf(lcd_putc, "     " );

     
      // Ajout de l'incrément et affichage de la valeur
      switch(_menu.active)
      {
        case MENU_FORME:
			lcd_gotoxy( 16, 1);
            
            if(data->Forme == 0 && incr == -1)
            {
               data->Forme = FORME_MAX;
            }
            else
            {
               data->Forme += incr * FORME_INCR;
            }
            
            
            if(data->Forme > FORME_MAX)
            {
               data->Forme = FORME_MIN;
            }
            else
            {
               // Do nothing
            }
            
            printf(lcd_putc, "%s",   MENU_VALUE[data->Forme] );
            break;
        case MENU_FREQ:
            lcd_gotoxy( 16, 2);
            data->Frequence += FREQUENCY_INCR * incr;
            
            if(data->Frequence > FREQUENCY_MAX)
            {
               data->Frequence = FREQUENCY_MIN;
            }
            else if(data->Frequence < FREQUENCY_MIN)
            {
               data->Frequence = FREQUENCY_MAX;
            }
            else
            {
               // Do nothing
            }
            printf(lcd_putc, "%ld", data->Frequence);
            break;
        case MENU_AMPL:
            lcd_gotoxy( 16, 3);
            data->Amplitude += incr * AMPLITUDE_INCR;
            
            if(data->Amplitude > AMPLITUDE_MAX)
            {
               data->Amplitude = AMPLITUDE_MIN;
            }
            else if(data->Amplitude < AMPLITUDE_MIN)
            {
               data->Amplitude = AMPLITUDE_MAX;
            }
            else
            {
               // Do nothing
            }
            
            printf(lcd_putc, "%ld",   data->Amplitude);
            break;
        case MENU_OFFSET:
            lcd_gotoxy( 16, 4);
            data->Offset += incr * OFFSET_INCR;
            
            if(data->Offset > OFFSET_MAX)
            {
               data->Offset = OFFSET_MIN;
            }
            else if(data->Offset < OFFSET_MIN)
            {
               data->Offset = OFFSET_MAX;
            }
            else
            {
               // Do nothing
            }
            
            printf(lcd_putc, "%ld",   data->Offset);
            break;
        default:
            break;
      }
}

// Vérouille le menu. Si clearPressed vaut 1, annule les modifications sinon elle les enregistres
void menu_set_lock(S_ParamGen * data, uint8_t clearPressed)
{
	_menu.lock = 1;
	menu_set_mark(LOCK_MARK);
	if(clearPressed)
	{
		switch(_menu.active)
		{
			case MENU_FORME:
				data->Forme = _menu.old;
				break;
			case MENU_FREQ:
				data->Frequence = _menu.old;
				break;
			case MENU_AMPL:
				data->Amplitude = _menu.old;
				break;
			case MENU_OFFSET:
				data->Offset = _menu.old;
				break;
			default:
				break;
		} 
		update_value_menu(0, data );
	}
	else
	{
		// Do nothing
	}
   
}

// Initialise les valeurs du menu
void menu_init_value(S_ParamGen * data)
{
   lcd_gotoxy( 16, 1);
   printf(lcd_putc, "     " );
   lcd_gotoxy( 16, 1);
   printf(lcd_putc, "%s",   MENU_VALUE[data->Forme] );


   lcd_gotoxy( 16, 2);
   printf(lcd_putc, "     " );
   lcd_gotoxy( 16, 2);
   printf(lcd_putc, "%ld",  data->Frequence);

   lcd_gotoxy( 16, 3);
   printf(lcd_putc, "     " );
   lcd_gotoxy( 16, 3);
   printf(lcd_putc, "%ld",  data->Amplitude);

   lcd_gotoxy( 16, 4);
   printf(lcd_putc, "     " );
   lcd_gotoxy( 16, 4);
   printf(lcd_putc, "%ld",  data->Offset);
}

void menu_init_info_char(char line1, char line2, char line3, char line4 )
{
   lcd_gotoxy( 1, 1);
   printf(lcd_putc, "%c", line1 );
   lcd_gotoxy( 1, 2);
   printf(lcd_putc, "%c", line2 );
   lcd_gotoxy( 1, 3);
   printf(lcd_putc, "%c", line3 );
   lcd_gotoxy( 1, 4);
   printf(lcd_putc, "%c", line4 );
}

// Initialise le menu
void menu_init(S_ParamGen * data)
{
   // Affichage des parties fixes
   lcd_clear();
   lcd_gotoxy( 2, 1);
   printf(lcd_putc, "Forme       =" );
   lcd_gotoxy( 2, 2);
   printf(lcd_putc, "Freq   [Hz] =");
   lcd_gotoxy( 2, 3);
   printf(lcd_putc, "Ampl   [mV] =" );
   lcd_gotoxy( 2, 4);
   printf(lcd_putc, "Offset [mV] =" );
   
   // Initialisation de la structure menu
   menu_goto(0);
   menu_init_value(data);
   menu_set_lock(data, 0);
}

// Incrémente la ligne sélectionnée selon la valeur de incr
static inline void menu_add_to_active(int incr)
{
   menu_goto(_menu.active + incr);
}
