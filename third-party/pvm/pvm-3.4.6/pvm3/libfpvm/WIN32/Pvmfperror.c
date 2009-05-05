
/* $Id: Pvmfperror.c,v 1.2 1998/09/21 20:27:01 pvmsrc Exp $ */

#include <stdio.h>
#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))

void 
#ifdef IMA_WIN32_WATCOM
pvmfperror (p_ptr,p_len, info)
#else
__fortran PVMFPERROR (p_ptr,p_len, info)
#endif

char * p_ptr; int p_len;
   int *info;
{
   static char *buf = 0;
   static int buflen = 0;
#ifndef WIN32
   char *malloc();
#endif
   /*
    * Have to have a NUL at the end of the string, and
    * the only way to do this portably is to copy the whole string
    * into a malloc'ed buffer.  We keep the buffer around for
    * future use rather than free'ing it each time we're done.
    */
   if (!buf)
      buf = malloc(buflen = p_len + 1);
   else
      if (p_len + 1 > buflen) {
         buflen = MAX(p_len + 1, buflen * 2);
         /* don't use realloc; it might cause old data to be copied */
         free(buf);
         buf = malloc(buflen);
      }
   if (!buf) {
      fprintf(stderr, "pvmfperror PvmNoMem");
      *info = PvmNoMem;
      return;
   }
   strncpy(buf, p_ptr, p_len);
   buf[p_len] = '\0';

   *info = pvm_perror(buf);
}

