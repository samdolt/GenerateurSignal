#define GEN_CARRE 0
#define GEN_TRIAN 1
#define GEN_SINUS 2
#define GEN_DDS 3


int16 Timer0Reload;
int32_t GENDATA[48];

void generator_sinus_init(S_ParamGen * data)
{
   uint8_t i;
   const double incr = 0.130899694;
   
   for(i = 0; i < 48; i++)
   {
      GENDATA[i] = (32759 * (1 +  ((sin(incr * i)  * (data->Amplitude / 10000.0) ))) ) -  ((data->Offset/1000.0) * (32759/5.0));
   }
}

void generator_carre_init(S_ParamGen * data)
{
   uint8_t i;
   
   for(i = 0; i < 24; i++)
   {
      GENDATA[i] = 32759  + (32759 * (data->Amplitude / 10000.0))  -  ((data->Offset/1000.0) * (32759/5.0))  ;
   }
   for(i = 0; i < 24; i++)
   {
      GENDATA[i+24] = 32759  - ( 32759  * (data->Amplitude / 10000.0)) -  ((data->Offset/1000.0) * (32759/5.0))  ;
   }
}

void generator_dds_init(S_ParamGen * data)
{
   int8_t i;
   for(i = 0; i<48; i++)
   {
      GENDATA[i] = 32759 - ((32759.0 * (i-24) / 48.0) * 2 * (data->Amplitude / 10000.0)) -  ((data->Offset/1000.0) * (32759/5.0)) ;
   }
}

void generator_trian_init(S_ParamGen * data)
{
   int8_t i;
   for(i = 0; i<24; i++)
   {
      GENDATA[i] = 32759 - ((32759.0 * (i-12) / 24.0) * 2 * (data->Amplitude / 10000.0)) -  ((data->Offset/1000.0) * (32759/5.0)) ;
   }
   for(i = 0; i<24; i++)
   {
      GENDATA[i+24] = 32759 + ((32759.0 * (i-12) / 24.0) * 2 * (data->Amplitude / 10000.0)) -  ((data->Offset/1000.0) * (32759/5.0)) ;
   }
}



void generator_update(S_ParamGen * data)
{
  int i;
   Timer0Reload = 65536  - ((double)( (double) 1.0/(uint32_t) (data->Frequence  * 48.0)*1000000) / (double) 0.0833333333333333333333333333);
   
   switch(data->Forme)
   {
      case GEN_SINUS:
         generator_sinus_init(data);
         break;
      case GEN_CARRE:
         generator_carre_init(data);
         break;
      case GEN_DDS:
         generator_dds_init(data);
         break;
      case GEN_TRIAN:
         generator_trian_init(data);
         break;
      default:
         break;
   }
   
   for(i = 0; i < 48 ; i++)
   {
      if (GENDATA[i] > 65518)
      {
         GENDATA[i] = 65518;
      }
      else if (GENDATA[i] < 0)
      {
         GENDATA[i] = 0;
      }
   }

   
  // set_timer0(65536 - 2500);
}


