/*   $Source: /var/local/cvs/gasnet/other/amxtests/testam.c,v $
 *     $Date: 2009/04/01 23:46:06 $
 * $Revision: 1.14 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "testam.h"

int false = 0;
/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int partner;
  int iters=0, polling = 1, i;

#if defined(AMUDP)
  putenv((char*)"A=A");
  putenv((char*)"B=B");
  putenv((char*)"C=C");
  putenv((char*)"ABC=ABC");
  putenv((char*)"AReallyLongEnvironmentName=A Really Long Environment Value");
#endif

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 2, "iters (Poll/Block)");

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 1;

  if (argc > 2) {
    switch(argv[2][0]) {
      case 'p': case 'P': polling = 1; break;
      case 'b': case 'B': polling = 0; break;
      default: printf("polling must be 'P' or 'B'..\n"); AMX_SPMDExit(1);
    }
  }

  /* setup handlers */
  SETUP_ALLAM();

  setupUtilHandlers(ep, eb);
  
  VMsegsz = 2*sizeof(testam_payload_t)*NUMHANDLERS_PER_TYPE;
  VMseg = malloc(VMsegsz);
  memset(VMseg, 0, VMsegsz);
  AM_Safe(AM_SetSeg(ep, VMseg, VMsegsz));

  if (false) { /* don't actually call these, just ensure they link properly */
    AMX_SPMDSetExitCallback(NULL);
    AMX_SPMDgetenvMaster("PATH");
    (void) AMX_SPMDIsWorker(argv);
    AMX_SPMDAllGather(NULL, NULL, 0);
    AMX_SPMDkillmyprocess(0);
  }

  /* barrier */
  AM_Safe(AMX_SPMDBarrier());

  partner = (MYPROC + 1)%NUMPROCS;

  /* compute */

  for (i=0; i < iters; i++) {

    ALLAM_REQ(partner);

    while (!ALLAM_DONE(i+1)) {
      if (polling) {
        AM_Safe(AM_Poll(eb));
      } else {
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY));
        AM_Safe(AM_WaitSema(eb));
        AM_Safe(AM_Poll(eb));
      }
    }
  }

#if defined(AMUDP)
  if (strcmp(AMX_SPMDgetenvMaster("A"),"A")) 
    FATALERR("Environment value mismatch on P%i\n", MYPROC);
  if (strcmp(AMX_SPMDgetenvMaster("B"),"B")) 
    FATALERR("Environment value mismatch on P%i\n", MYPROC);
  if (strcmp(AMX_SPMDgetenvMaster("C"),"C"))
    FATALERR("Environment value mismatch on P%i\n", MYPROC);
  if (strcmp(AMX_SPMDgetenvMaster("ABC"),"ABC"))
    FATALERR("Environment value mismatch on P%i\n", MYPROC);
  if (strcmp(AMX_SPMDgetenvMaster("AReallyLongEnvironmentName"),"A Really Long Environment Value"))
    FATALERR("Environment value mismatch on P%i\n", MYPROC);
#endif

  /* barrier */
  AM_Safe(AMX_SPMDBarrier());

  printGlobalStats();

  AM_Safe(AMX_SPMDBarrier());

  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
