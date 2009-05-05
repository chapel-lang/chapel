#include<process.h>
#include<stdio.h>
#include<stdlib.h>

int os2_spawn( char *path, char **argv, int nenv, char **env, 
               char *debugger)
//-----------------------------------------------------------
 { char *Arg[1024],*Env[1024],**d,**s,*idstr="PVMEPID=",buf[32];
 
   sprintf(buf,"%s%i",idstr,0);

   d=Arg; s=argv;
   if (debugger) { *d = getenv("PVM_DEBUGGER");
                   if(*d==0)*d=debugger;
                   d++;
                  }
   *d++=path; s++; while(*s)*d++=*s++; *d=0;

   d=Env; s=environ; 
   while(*s) { if(strncmp(*s,idstr,strlen(idstr)))*d++=*s;
               else  *d++=buf;
               s++; 
             }
   s=env; while(nenv--)*d++=*s++;  *d=0;

   return spawnve(P_NOWAIT,path,Arg,Env);
 }

