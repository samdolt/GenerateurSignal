
// driver pour le LCD BC1602A
// cet affichage est un affichage 4 lignes de 20 caractères

// Les fonctions suivantes sont accessibles:
//
// lcd_init() : routine d'initialisation de l'affichage avec effacement
//              et retour à la position 1,1
//
// lcd_gotoxy( BYTE x, BYTE y ): positionnement du curseur à la position
//              x,y sachant que le caractère en haut à gauche est le 1,1
//
// lcd_putc( char c): écrit le caractère c à la position actuelle du curseur
//              et déplace le curseur au caractère suivant
//              Les caractères suivants ont une signification particulière:
//               \f  efface l'écran et le curseur revient à la position 1,1
//               \n  envoie le curseur au début de la deuxième ligne
//               \b  retour en arrière d'une position
//
// lcd_put_string_ram( char *ptr_char ) : écrit le string dont l'adresse de
//              départ est passée en paramètre
//              Le string doit avoir été déclaré en ram avec une définition
//              du genre:
//                byte string[] = "Bonjour à tous";
//
//
// lcd_getc( BYTE x, BYTE y): retourne le caractère affiché à la position x,y
//
// lcd_bl_on(): allume le rétro-éclairage
//
// lcd_bl_off(): éteint le rétro-éclairage
//
// TOUTES LES INFORMATIONS NECESSAIRES (AINSI QUE DES PROTOTYPES DE FONCTIONS)
// A L'AFFICHAGE DE STRINGS DEFINIS EN EPROM SONT DONNEES A LA FIN DU FICHIER
//
//---------------------------------------------------------------------------
// Les connexions avec l'affichage LCD sont:
//     PortE0  LCD RS
//     PortE1  LCD R/W
//     PortE2  LCD ENABLE
//     PortE3  LCD BACKLIGHT
//     PortE4  LCD D4
//     PortE5  LCD D5
//     PortE6  LCD D6
//     PortE7  LCD D7
//
//   Les pins D0-D3 du LCD ne sont pas utilisées.

struct lcd_pin_map {                 // This structure is overlayed
           BOOLEAN rs;               // on to an I/O port to gain
           BOOLEAN rw;               // access to the LCD pins.
           BOOLEAN enable;           // The bits are allocated from
           BOOLEAN back_light;       // low order up.  RS will
           int     data : 4;         // be pin D0.
        } lcd;

#byte lcd = 0xF84                   // Cette structure est affectée au port E
#define set_tris_lcd(x) set_tris_e(x)


#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 or 4 lines


const int8 taddrLines[5] = { 0,
                              0x00,    // ligne 1
                              0x40,    // ligne 2
                              0x14,    // ligne 3
                              0x54 };  // ligne 4

BYTE const LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6};
                             // These bytes need to be sent to the LCD
                             // to start it up.

struct lcd_pin_map const LCD_WRITE = {0,0,0,0,0}; // Pour écrire, toutes les pattes sont en sortie
struct lcd_pin_map const LCD_READ = {0,0,0,0,15}; // Pour lire, les pattes de datas sont en entrée


BYTE lcd_read_byte() {
      BYTE low,high;
      set_tris_lcd(LCD_READ);
      lcd.rw = 1;
      delay_cycles(1);
      lcd.enable = 1;
      delay_cycles(1);
      high = lcd.data;
      lcd.enable = 0;
      delay_cycles(1);
      lcd.enable = 1;
      delay_us(1);
      low = lcd.data;
      lcd.enable = 0;
      set_tris_lcd(LCD_WRITE);
      return( (high<<4) | low);
}

void lcd_send_nibble( BYTE n ) {
      lcd.data = n;
      delay_cycles(1);
      lcd.enable = 1;
      delay_us(2);
      lcd.enable = 0;
}

void lcd_send_byte( BYTE address, BYTE n ) {

      lcd.rs = 0;
      while ( bit_test(lcd_read_byte(),7) ) ;
      lcd.rs = address;
      delay_cycles(1);
      lcd.rw = 0;
      delay_cycles(1);
      lcd.enable = 0;
      lcd_send_nibble(n >> 4);
      lcd_send_nibble(n & 0xf);
}

void lcd_init() {
    BYTE i;
    set_tris_lcd(LCD_WRITE);
    lcd.rs = 0;
    lcd.rw = 0;
    lcd.enable = 0;
    delay_ms(15);
    for(i=1;i<=3;++i) {
       lcd_send_nibble(3);
       delay_ms(5);
    }
    lcd_send_nibble(2);
    for(i=0;i<=3;++i)
       lcd_send_byte(0,LCD_INIT_STRING[i]);
}

void lcd_gotoxy( BYTE x, BYTE y) {
   BYTE address;

   // sans control
   address = taddrLines[y];
   
   address+=x-1;
   lcd_send_byte(0,0x80|address);
}

void lcd_putc( char c) {
   switch (c) {
     case '\f'   : lcd_send_byte(0,1);
                   delay_ms(2);
                                           break;
     case '\n'   : lcd_gotoxy(1,2);        break;
     case '\b'   : lcd_send_byte(0,0x10);  break;
     default     : lcd_send_byte(1,c);     break;
   }
}

void lcd_put_string_ram( char *ptr_char ) {
  while (*ptr_char != 0) {
    lcd_putc(*ptr_char);
    ptr_char++;
  }
}

char lcd_getc( BYTE x, BYTE y) {
   char value;

    lcd_gotoxy(x,y);
    while ( bit_test(lcd_read_byte(),7) ); // wait until busy flag is low
    lcd.rs=1;
    value = lcd_read_byte();
    lcd.rs=0;
    return(value);
}

void lcd_bl_on() {
  lcd.back_light=1;
}

void lcd_bl_off() {
  lcd.back_light=0;
}



/*
  La fonction lcd_put_string_ram écrit sur le LCD un string dont l'adresse du premier caractère est
  passée en paramètre.
  Du point de vue de la librairie, l'exécution est aisée car il suffit de déclarer un pointeur
  sur byte, pointeur auquel on affecte l'adresse passée en paramètre, et d'écrire les bytes
  pointés (en incrémentant l'adresse à chaque passage) dans le LCD jusqu'à ce qu'on trouve le
  caractère de fin de string 0x00.

  Ceci fonctionne bien mais présente l'inconvénient de devoir utiliser des strings définis en RAM.
  Un string défini en ram se déclare par:
      byte string[] = "exemple";
  Ceci non seulement utilise de la ram, qui souvent est limitée dans les microcontrôleurs, mais
  de plus exige son initialisation au power-up ou au reset. Cette initialisation utilise du code et
  prend un peu de temps.
  Il est bien entendu indispensable d'utiliser des strings en ram si on désire les modifier en cours
  d'exécution.

  Si on désire économiser de la ram et du code (et du temps) à l'éxécution, et pour autant qu'on ne
  veuille pas modifier un string durant l'exécution, il est possible de le définir en eprom par:
      const byte string[] = "exemple";
  Ce string se trouvera alors en eprom.

  MALHEUREUSEMENT, AVEC LES PICS (AUTRES QUE LES DSPICS), ON NE PEUT PAS POINTER SUR L'EPROM, ce qui
  ne nous permet pas d'utiliser la fonction lcd_put_string_ram pour écrire un string enregistré en
  eprom sur le lcd.

  Par contre, si on ne peut pas pointer sur un string contenu en eprom, il est possible d'utiliser
  la notation tableau string[2] qui nous donnera le troisième caractère du string.
  Il nous faut donc trouver une solution pour utiliser cette technique.

  La solution proposée est la suivante:

  1) On définit les strings en mémoire eprom:
     const int8 string_a_tous []   = {" a tous "};
     const int8 string_starter_kit []    = {"StarterKit18F4x2"};

     CECI DOIT ETRE FAIT DANS LE PROGRAMME APPLICATION

  2) On déninit un type énumération qui reprend les noms de ces lignes
     typedef enum {e_string_a_tous, e_string_starter_kit} tstring_rom;

     CECI DOIT ETRE FAIT DANS LE PROGRAMME APPLICATION

  Que l'on utilisera pour retourner le caractère à afficher.
  pour trouver le bon caractère, on utilisera une fonction qui aura en entrée une valeur du type
  énuméré donnant la ligne que l'on désire afficher et l'ordre du caractère dans cette ligne
  cette fonction retournera le caractère à afficher.

  3) Pour l'affichage d'une ligne, on effectuera une boucle qui commencera par le premier caractère
     de la ligne et qui prendra les caractères suivants jusqu'au 00h.
     Cette fonction reçoit en paramètre une variable de type tstring_rom

     3a) fonction display_string_rom:
     

//********************************************************************************************
     void display_string_rom (tstring_rom string_rom) {
     int char_prov, char_cnt;
       char_cnt = 0;
       do {
         char_prov = get_string_rom_char(string_rom, char_cnt);
         if (char_prov != 0) {
           lcd_putc(char_prov);
         }
         char_cnt++;
       }
       while (char_prov != 0);
     }
     

//********************************************************************************************

     COMME ON UTILISE LE TYPE tstring_rom QUI EST DEFINI DANS LE PROGRAMME APPLICATION,
     CELA DOIT ETRE FAIT DANS LE PROGRAMME APPLICATION

      Cette fonction utilise une fonction get_string_rom_char qui rend le caractère du string.
      Cette fonction doit se trouver dans le programme d'application car elle utilise un
      switch case qui reprend les noms des strings!!!!!

     3b) fonction get_string_rom_char:
     

//********************************************************************************************

     int8 get_string_rom_char ( tstring_rom string_rom, int ordre) {
       switch (string_rom) {
         case e_string_a_tous: return (string_a_tous[ordre]);
         break;
         case e_string_starter_kit: return (string_starter_kit[ordre]);
         break;
       }
     }
     

//********************************************************************************************

     CECI DOIT ETRE FAIT DANS LE PROGRAMME APPLICATION

     

******************************************************************************************
     *
     *      E N     R E S U M E
     *
     * - définir les strings en eprom par const...
     * - définir le type énumération tstring_rom
     * - copier les 2 fonctions dans le programme application (au début ?!?)
     *   attention!! copier get_string_rom_char AU-DESSUS de 

display_string_rom
     * - ajuster la fonction get_string_rom_char en fonction des strings et de tstring_rom
     *
     *
     *    ET CELA FONCTIONNE
     *
*/
