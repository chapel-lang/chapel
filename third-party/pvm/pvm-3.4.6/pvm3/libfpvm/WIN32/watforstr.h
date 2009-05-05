
/* $Id: watforstr.h,v 1.1 1997/06/26 21:30:45 pvmsrc Exp $ */

#ifndef WATFORSTR_H
#define WATFORSTR_H

/*
  This is the structure the Watcom-Fortran compiler
   uses to pass strings
*/
typedef struct WatcomFortranStrDef
{
   char* strP;
   int   len;
} WatcomFortranStr;
   
#endif

