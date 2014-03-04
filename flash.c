// Samuel Dolt et Claudio Palmari
// Sauvegarde en mémoire programme d'une structure S_ParamGen

// Constante utilisée pour la vérification de la validité de la lecture
const uint32_t PROGMEM_MAGIC = 0x0F1E2D3C;

// Variable utilisée pour stocker l'adresse de la mémoire programme utilisée
int32 progmem_addr;

void progmem_init(void)
{
	// Cette fonction calcul l'adresse de la mémoire programme qui est utilisée
	// Cette adresse correspond à l'avant dernier blocs disponible.
	int16 progmem_erase_size;
	int16 progmem_write_size;
	int32 progmem_size;
	int32 progmem_blocs;
   
	// Demande des informations aux compilateurs
	progmem_erase_size = getenv("FLASH_ERASE_SIZE");
	progmem_write_size = getenv("FLASH_WRITE_SITE");
	progmem_size = getenv("PROGRAM_MEMORY");
	
	// Calcul du nombre de bloc mémoire
	progmem_blocs = progmem_size / progmem_erase_size;
	
	// Calcul de l'adresse mémoire de l'avant dernier bloc
	progmem_addr = (progmem_blocs -1)* progmem_erase_size;
}

void progmem_save_data(S_ParamGen * data)
{
	// Enregistre la structure à l'adresse data dans la mémoire programme à l'adresse progmem_addr
   write_program_memory(progmem_addr, data, sizeof(S_ParamGen));
}

void progmem_load_data(S_ParamGen * data)
{
	// Cette fonction lis la mémoire programme à l'adresse progmem_addr
	// puis vérifie la validité des données. Si tous ce passe bien, les données
	// sont copiées dans la stucture data passée par référence
	
	S_ParamGen progmem_data;

	// Lecture de la mémoire
	read_program_memory(progmem_addr, &progmem_data, sizeof(S_ParamGen));
	
	// Vérification du champ Magic
	if(progmem_data.Magic == PROGMEM_MAGIC)
	{
		// Si tous ce passe bien, on copie les données dans data
		memcpy(data, &progmem_data, sizeof(S_ParamGen));
	}
	else
	{
      // Si la validation échoue, on ne fait rien
	}
}




