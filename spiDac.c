// Fichier SpiDac.c
// Gestion du DAC LTC2604 du kit 18F67J50
// DAC sur Spi2
// Version avec SpiClk = 12 MHz

#bit CKP = 0xF6C.4

// Initialisation du LTC2604
void InitLTC2604(void)  {
  //Initialisation SPI DAC
   output_high(CS_DAC);
   // Impulsion reste du DAC
   output_low(ResetCarte);
   delay_us(500);
   output_high(ResetCarte);
   // LTC2604 MAX 50 MHz 12 HHz avec Div 4 = div min
   setup_spi2(SPI_MASTER | SPI_H_TO_L | SPI_CLK_DIV_4); 
   CKP = 0;
}

// Envoi d'une valeur sur le DAC  LTC2604
// Indication du canal 0 à 3
void WriteDac(uint16 ValDAc, uint8 Noch)
{
   
  //Déclaration des variables
   uint8 MSB;
   uint8 LSB;
   
   //Sélection du canal
   //3 -> Set and Update, 0/1/2/3 Sélection canal A/B/C/D, F tous canaux
   NoCh = NoCh + 0x30;
   
   // selon Canal
   MSB =  ValDAc >> 8;
   LSB =  ValDAc;
   output_low(CS_DAC);
   spi_write2(NoCh);
   spi_write2(MSB);
   spi_write2(LSB);
   //Fin de transmission
   output_high(CS_DAC); 
}



