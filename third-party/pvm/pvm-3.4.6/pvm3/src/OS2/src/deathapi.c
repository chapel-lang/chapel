/* Copyright (C)1996 by Holger.Veit@gmd.de */
/* This code may be freely used in own software */

#define INCL_DOS
#define INCL_ERRORS
#include <os2.h>
#include <stdio.h>

/* 
#include "death.h"
*/
#include<signal.h>

static
APIRET death(PID pid)
{
	APIRET rc;
	HFILE hfd;
	ULONG action,plen;
	USHORT param;

	if ((rc=DosOpen((PSZ)"/dev/fastio$", (PHFILE)&hfd, (PULONG)&action,
	    (ULONG)0, FILE_SYSTEM, FILE_OPEN,
	    OPEN_SHARE_DENYNONE|OPEN_FLAGS_NOINHERIT|OPEN_ACCESS_READONLY,
	    (ULONG)0))) {
		fputs("Error opening fastio$ driver...\n",stderr);
		fputs("Please install xf86sup.sys in config.sys!\n",stderr);
		return rc;
	}

	param = pid;

	if ((rc=DosDevIOCtl(hfd, (ULONG)0x76, (ULONG)0x65,
	    (PULONG*)&param,sizeof(USHORT),&plen,
	    NULL, 0, NULL)) != 0) {
		fprintf(stderr,"fastio$ driver: kill(%i,9) failed rc=%i\n",pid,rc); 
                DosClose(hfd);
                return rc;
        }
	DosClose(hfd);
	return 0;
}

int os2_kill(int pid, int sig)
{ if(sig==SIGTERM||sig==SIGKILL||sig==SIGINT||sig==SIGBREAK||sig==SIGHUP)
    return death(pid);
  else
    return kill(pid,sig);     
}

