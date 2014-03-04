const uint32_t PROGMEM_MAGIC = 0x0F1E2D3C;

int16 progmem_erase_size, progmem_write_size;
int32 progmem_addr, progmem_blocs;

S_ParamGen progmem_data;

void progmem_init()
{
   progmem_erase_size = getenv("FLASH_ERASE_SIZE");
   progmem_write_size = getenv("FLASH_WRITE_SITE");
   progmem_addr = getenv("PROGRAM_MEMORY");
   progmem_blocs = progmem_addr / progmem_erase_size;
   progmem_addr = (progmem_blocs -1)* progmem_erase_size;
}

void progmem_load_data(S_ParamGen * data)
{
   read_program_memory(progmem_addr, &progmem_data, sizeof(S_ParamGen));
   if(progmem_data.Magic == PROGMEM_MAGIC)
   {
      memcpy(data, &progmem_data, sizeof(S_ParamGen));
   }
   else
   {
      // Do nothing
   }
}

void progmem_save_data(S_ParamGen * data)
{
   write_program_memory(progmem_addr, data, sizeof(S_ParamGen));
}


