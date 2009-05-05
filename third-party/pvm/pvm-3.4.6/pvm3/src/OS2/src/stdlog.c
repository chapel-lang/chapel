#include<stdio.h>
#include<stdarg.h>

void stdlog(char *fmt,...)
/*======================*/
{  FILE *F;
   char buf[4086]="";
   va_list argptr;
   va_start(argptr, fmt);
   vsprintf(buf, fmt, argptr);
   va_end(argptr);
   F=fopen("c:/tmp/log.pvm","at");
   if(F){ fprintf(F,buf);
          fclose(F);
        }
}
