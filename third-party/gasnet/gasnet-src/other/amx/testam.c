/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/testam.c $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "testam.h"

#define verify(cond) do { \
  if (!(cond)) { fprintf(stderr,"%i: ERROR: verify(%s) failed at %s:%i\n", \
    MYPROC, #cond, __FILE__,__LINE__); fflush(stderr); \
  } } while(0)

volatile int false = 0;
/* ------------------------------------------------------------------------------------ */
ep_t check_ep;
en_t check_en;
tag_t check_tag;
void checkAMshort(void *token) {
  ep_t ep;
  en_t en;
  tag_t tag;
  int id = -1;
  AM_Safe(AMX_GetSourceId(token,&id));
  verify(id == 0);
  AM_Safe(AM_GetSourceEndpoint(token,&en));
  verify(AMX_enEqual(en,check_en));
  AM_Safe(AM_GetDestEndpoint(token,&ep));
  verify(ep == check_ep);
  AM_Safe(AM_GetMsgTag(token,&tag));
  verify(tag == check_tag);

  check_tag = 0;
}
/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int partner;
  int iters=0, polling = 1, i;

#if defined(AMUDP)
  const char *envvars[][3] = {
    #define DEFVAR(key,val) { key, val, key "=" val },
    DEFVAR("A","A")
    DEFVAR("B","B")
    DEFVAR("C","C")
    DEFVAR("ABC","ABC")
    DEFVAR("AReallyLongEnvironmentName","A Really Long Environment Value")
  };
  for (i=0; i < sizeof(envvars)/sizeof(envvars[0]); i++) {
    putenv((char*)(envvars[i][2]));
  }
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

  { /* misc tests for functions possibly not covered elsewhere */
    int numhand = AM_MaxNumHandlers(); verify(numhand >= 256);
    int n = 0;
    tag_t t,mytag;
    AM_Safe(AM_SetNumHandlers(ep, numhand));
    AM_Safe(AM_GetNumHandlers(ep, &n));
    verify(n >= numhand);
    AM_Safe(AM_GetTag(ep, &mytag));
    for (n=0; n < NUMPROCS; n++) {
      verify(AM_GetTranslationInuse(ep,n) == AM_OK);
      AM_Safe(AM_GetTranslationTag(ep,n,&t));
      if (n == MYPROC) verify(t == mytag);
    }
    verify(AM_GetTranslationInuse(ep,NUMPROCS) != AM_OK);
  }

  { /* test direct endpoint/bundle ops */
    eb_t eb1, eb2;
    ep_t ep1;
    en_t en1, en2;
    int i=0,n,m;
    handler_t h;
    size_t sz;
    void *vp=0;
    tag_t t1 = (tag_t)0x1234ABCDU;
    AM_Safe(AM_AllocateBundle(AM_SEQ,&eb1));
    AM_Safe(AM_AllocateBundle(AM_SEQ,&eb2));
    AM_Safe(AM_AllocateEndpoint(eb1,&ep1,&en1));
    check_ep = ep1;
    check_en = en1;

    AM_Safe(AM_GetNumTranslations(ep1, &n)); verify(n >= 256);
    AM_Safe(AM_MaxNumTranslations(&m)); verify(m >= 256);
    if (m > 1024) m = 1024;
    AM_Safe(AM_SetNumTranslations(ep1, m));
    AM_Safe(AM_GetNumTranslations(ep1, &n)); verify(n == m);

    AM_Safe(AM_MapAny(ep1,&i,en1,t1));
    verify(i >= 0 && i < m);
    verify(AM_GetTranslationInuse(ep1,i) == AM_OK);
    check_tag = 0;
    AM_Safe(AM_GetTranslationTag(ep1,i,&check_tag)); verify(t1 == check_tag);
    AM_Safe(AM_UnMap(ep1,i));
    verify(AM_GetTranslationInuse(ep1,i) != AM_OK);
    AM_Safe(AM_Map(ep1,i,en1,t1));
    AM_Safe(AM_MaxSegLength(&sz)); verify(sz >= sizeof(int));
    AM_Safe(AM_SetSeg(ep1,&n,sizeof(int)));
    AM_Safe(AM_SetTag(ep1,t1));

    AM_Safe(AM_MoveEndpoint(ep1,eb1,eb2));
    AM_Safe(AM_FreeBundle(eb1));
    verify(AM_GetTranslationInuse(ep1,i) == AM_OK);
    check_tag = 0;
    AM_Safe(AM_GetTranslationTag(ep1,i,&check_tag)); verify(t1 == check_tag);
    AM_Safe(AM_GetTranslationName(ep1,i,&en2)); verify(AMX_enEqual(en1, en2));
    AM_Safe(AM_GetSeg(ep1,&vp,&sz)); verify(sz == sizeof(int)); verify(vp == &n);
    AM_Safe(AM_GetNumTranslations(ep1, &n)); verify(n == m);
    check_tag = 0;
    AM_Safe(AM_GetTag(ep1,&check_tag)); verify(t1 == check_tag);

    AM_Safe(AM_SetExpectedResources(ep1,1,1));
    h = (handler_t)-1;
    AM_Safe(AM_SetHandlerAny(ep1,&h,checkAMshort));
    i = (int)h; // bug3459: avoid a warning
    verify(i >= 0 && i < AM_MaxNumHandlers());
    AM_Safe(AM_Request0(ep1,0,h));
    do {
      AM_Safe(AM_Poll(eb2));
    } while (check_tag);

    AM_Safe(AM_FreeEndpoint(ep1));
    AM_Safe(AM_FreeBundle(eb2));
  }

  /* setup handlers */
  SETUP_ALLAM();

  setupUtilHandlers(ep, eb);
  
  VMsegsz = 2*sizeof(testam_payload_t)*NUMHANDLERS_PER_TYPE;
  VMseg = malloc(VMsegsz);
  memset(VMseg, 0, VMsegsz);
  AM_Safe(AM_SetSeg(ep, VMseg, VMsegsz));

  if (false) { /* don't actually call these, just ensure they link properly */
    static char *ignore;
    AMX_SPMDSetExitCallback(NULL);
    ignore += ignore - AMX_SPMDgetenvMaster("PATH");
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
  for (i=0; i < sizeof(envvars)/sizeof(envvars[0]); i++) {
    const char *key = envvars[i][0];
    const char *val = envvars[i][1];
    const char *actual = AMX_SPMDgetenvMaster(key);
    if (!actual) actual = "<undef>";
    if (strcmp(val, actual))
      printf("P%i: ERROR Environment value mismatch: %s='%s'\n", MYPROC, key, actual);
  }
#endif

  printf("Slave %i done.\n", MYPROC);
  fflush(stdout);

  /* barrier */
  AM_Safe(AMX_SPMDBarrier());

  printGlobalStats();

  AM_Safe(AMX_SPMDBarrier());

  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
