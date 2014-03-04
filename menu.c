#define LOCK_MARK '*'
#define UNLOCK_MARK '>'

#define MENU_FORME 0
#define MENU_FREQ 1
#define MENU_AMPL 2
#define MENU_OFFSET 3

const char MENU_VALUE[4][6] = {"CARRE","TRIAN","SINUS","DDS"};

const uint16_t FORME_INCR = 1;
const uint16_t FORME_MIN = 0;
const uint16_t FORME_MAX = 3;

const uint16_t FREQUENCY_INCR = 10;
const uint16_t FREQUENCY_MIN = 10;
const uint16_t FREQUENCY_MAX = 990;

const uint16_t AMPLITUDE_INCR = 50;
const uint16_t AMPLITUDE_MIN = 0;
const uint16_t AMPLITUDE_MAX = 10000;

const uint16_t OFFSET_INCR = 50;
const int16_t OFFSET_MIN = -5000;
const int16_t OFFSET_MAX = 5000;

struct menu_s
{
   int8_t active;
   int8_t lock;
   char mark;
   uint16_t old;
}_menu;


static inline bool menu_is_locked(void)
{
   return _menu.lock;
}

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

static inline char menu_get_mark(void)
{
   return _menu.mark;
}

static inline void menu_set_mark(char mark)
{
   _menu.mark = mark;
   update_mark();
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

     
         // Incr
        // generator.Forme = (E_FormesSignal) SignalCarre;
  // generator.Frequence = 100;
  // generator.Amplitude = 10;
  // generator.Offset = 0;
      // Affichage de la valeur
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
void menu_init_value(S_ParamGen * data)
{
            lcd_gotoxy( 16, 1);
             printf(lcd_putc, "%s",   MENU_VALUE[data->Forme] );

            lcd_gotoxy( 16, 2);
            printf(lcd_putc, "%ld",  data->Frequence);

            lcd_gotoxy( 16, 3);
            printf(lcd_putc, "%ld",  data->Amplitude);

            lcd_gotoxy( 16, 4);
            printf(lcd_putc, "%ld",  data->Offset);
}

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

static inline void menu_add_to_active(int incr)
{
   menu_goto(_menu.active + incr);
}




