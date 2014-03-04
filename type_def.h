// ETML Ecole Technique               					      
// D�finitions des types de donn�es    						  
// Cr�ation 18.04.2002 CHR   V 1.0    						  
// R�vision 23.10.2003 CHR   V 2.0  (adaptation C51 et CCS C) 

// Ce fichier type_def.h est � copier dans chaque projet et 
// est � inclure dans chaque module 

#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#ifdef __C51__

// definitions pour KEIL C51

typedef bit bool;					// 1 bit flag

typedef signed char sint8;			// 8 bits sign�
typedef unsigned char uint8;		// 8 bits non sign�

typedef signed short sint16;		// 16 bits sign�
typedef unsigned short uint16;		// 16 bits non sign�

typedef signed long sint32;			// 32 bits sign�
typedef unsigned long uint32;		// 32 bits non sign�

typedef float real32;			// 32 bits float

#else
// definitions pour CCS C

typedef int1 bool;				// 1 bit flag

typedef signed int8 sint8;		// 8 bits sign�
typedef int8 uint8;				// 8 bits non sign�

typedef signed int16 sint16;	// 16 bits sign�
typedef int16 uint16;			// 16 bits non sign�

typedef signed int32 sint32;	// 32 bits sign�
typedef int32 uint32;			// 32 bits non sign�

typedef float real32;			// 32 bits float

#endif

#endif
