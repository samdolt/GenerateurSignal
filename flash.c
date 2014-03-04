// Samuel Dolt et Claudio Palmari
// Sauvegarde en m�moire programme d'une structure S_ParamGen

// Constante utilis�e pour la v�rification de la validit� de la lecture
const uint32_t PROGMEM_MAGIC = 0x0F1E2D3C;

// Variable utilis�e pour stocker l'adresse de la m�moire programme utilis�e
int32 progmem_addr;

void progmem_init(void)
{
	// Cette fonction calcul l'adresse de la m�moire programme qui est utilis�e
	// Cette adresse correspond � l'avant dernier blocs disponible.
	int16 progmem_erase_size;
	int16 progmem_write_size;
	int32 progmem_size;
	int32 progmem_blocs;
   
	// Demande des informations aux compilateurs
	progmem_erase_size = getenv("FLASH_ERASE_SIZE");
	progmem_write_size = getenv("FLASH_WRITE_SITE");
	progmem_size = getenv("PROGRAM_MEMORY");
	
	// Calcul du nombre de bloc m�moire
	progmem_blocs = progmem_size / progmem_erase_size;
	
	// Calcul de l'adresse m�moire de l'avant dernier bloc
	progmem_addr = (progmem_blocs -1)* progmem_erase_size;
}

void progmem_save_data(S_ParamGen * data)
{
	// Enregistre la structure � l'adresse data dans la m�moire programme � l'adresse progmem_addr
   write_program_memory(progmem_addr, data, sizeof(S_ParamGen));
}

void progmem_load_data(S_ParamGen * data)
{
	// Cette fonction lis la m�moire programme � l'adresse progmem_addr
	// puis v�rifie la validit� des donn�es. Si tous ce passe bien, les donn�es
	// sont copi�es dans la stucture data pass�e par r�f�rence
	
	S_ParamGen progmem_data;

	// Lecture de la m�moire
	read_program_memory(progmem_addr, &progmem_data, sizeof(S_ParamGen));
	
	// V�rification du champ Magic
	if(progmem_data.Magic == PROGMEM_MAGIC)
	{
		// Si tous ce passe bien, on copie les donn�es dans data
		memcpy(data, &progmem_data, sizeof(S_ParamGen));
	}
	else
	{
      // Si la validation �choue, on ne fait rien
	}
}




