/* $Id: rtinfo.c,v 1.1 2003-03-20 00:57:27 d3h325 Exp $ Run-time system configuration */

#include <stdio.h>
#include <string.h>

/*\ determine number of CPUs on the current SMP node- Linux version for now 
\*/
int armci_getnumcpus(void)
{
int numproc=0;
FILE* fp;
char line[80];
   fp=fopen("/proc/cpuinfo","r");
   if(fp==NULL) return -1;
   while(!feof(fp)){
     fgets(line,80,fp);
     if(strncmp(line,"processor",9)==0) numproc++;
   }
   fclose(fp);
   return(numproc);
}

