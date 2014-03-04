// ETML Ecole Technique               					      
// Définitions des types de données    						  
// Création 18.04.2002 CHR   V 1.0    						  
// Révision 23.10.2003 CHR   V 2.0  (adaptation C51 et CCS C) 

// Ce fichier type_def.h est à copier dans chaque projet et 
// est à inclure dans chaque module 

#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#ifdef __C51__

// definitions pour KEIL C51

typedef bit bool;					// 1 bit flag

typedef signed char sint8;			// 8 bits signé
typedef unsigned char uint8;		// 8 bits non signé

typedef signed short sint16;		// 16 bits signé
typedef unsigned short uint16;		// 16 bits non signé

typedef signed long sint32;			// 32 bits signé
typedef unsigned long uint32;		// 32 bits non signé

typedef float real32;			// 32 bits float

#else
// definitions pour CCS C

typedef int1 bool;				// 1 bit flag

typedef signed int8 sint8;		// 8 bits signé
typedef int8 uint8;				// 8 bits non signé

typedef signed int16 sint16;	// 16 bits signé
typedef int16 uint16;			// 16 bits non signé

typedef signed int32 sint32;	// 32 bits signé
typedef int32 uint32;			// 32 bits non signé

typedef float real32;			// 32 bits float

#endif

#endif
