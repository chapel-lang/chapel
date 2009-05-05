
static char rcsid[] =
	"$Id: pvm_shmd_stat.c,v 1.1 1999/03/04 21:30:52 pvmsrc Exp $";

#include "pvm3.h"
#include "shmd.h"
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif


main ()
{
        int             numsegs=0;
        char    hname[MAXHOSTNAMELEN];
        char    shmdname[1024];
        int isitok=0; /* ok or not ? */
	int mytid, i, j, k, srbuf, localid, shmdtid;
	int id;

                mytid = pvm_mytid();
                if (mytid<0) /* ops no pvm... */
                        exit (-1);

        bzero (shmdname, 1024);
        gethostname(hname, MAXHOSTNAMELEN);
        strcat (shmdname, "pvm_shmd:");
        strcat (shmdname, hname);


        srbuf = pvm_setrbuf(0);
        if ( pvm_recvinfo(shmdname, 0, PvmMboxDefault) == PvmNotFound ) {
                fprintf(stderr,"No pvm_shmd\n");
                exit (-1);
        }
        pvm_upkint(&shmdtid,1,1);
        pvm_upkint(&id,1,1);
        pvm_setrbuf(srbuf);


        pvm_initsend(PvmDataDefault);
        pvm_pkint(&mytid,1,1);

	/* get block map is message tag 3 */
        pvm_send (shmdtid, 3);                  

	pvm_exit();

}

