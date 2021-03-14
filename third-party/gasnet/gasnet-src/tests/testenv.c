/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testenv.c $
 * Description: GASNet environment variable propagation test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

#include <test.h>

#ifndef TEST_VAR
#define TEST_VAR "GASNET_DUMMY"
#endif

#ifndef TEST_VAL
#define TEST_VAL "GASNet dummy value"
#endif

const char *expect_args[] = { "GASNet", "is", "Global Address Space Networking" };
int expect_argc = 1 + sizeof(expect_args)/sizeof(char *);

int main(int argc, char **argv) {
  char usagestr[255];
  char tmp[1024];
  int i;
  const char *startup_val = NULL;
  const char *running_val = NULL;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testenv", &argc, &argv, 0));
    startup_val = gasnet_getenv(TEST_VAR);
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  usagestr[0] = '\0';
  for (i=0; i < expect_argc-1; i++) {
    strcat(usagestr,"'");
    strcat(usagestr,expect_args[i]);
    strcat(usagestr,"' ");
  }
  test_init("testenv",0,usagestr);

  BARRIER();

  running_val = gasnet_getenv(TEST_VAR);

  BARRIER();

  if (!startup_val)
    ERR("gasnet_getenv('%s') between init/attach returned NULL", TEST_VAR);
  else if (strcmp(startup_val, TEST_VAL))
    ERR("gasnet_getenv('%s') between init/attach returned '%s', expected '%s'", 
        TEST_VAR, startup_val, TEST_VAL);

  if (!running_val)
    ERR("gasnet_getenv('%s') after attach returned NULL", TEST_VAR);
  else if (strcmp(running_val, TEST_VAL))
    ERR("gasnet_getenv('%s') after attach returned '%s', expected '%s'", 
        TEST_VAR, running_val, TEST_VAL);

  BARRIER();

  snprintf(tmp, sizeof(tmp), "argc=%i, argv[] = ", argc);
  for (i=0; i < argc; i++) {
    char tmp2[255];
    snprintf(tmp2, sizeof(tmp2), "%s'%s'", (i>0?", ":""), argv[i]);
    strcat(tmp, tmp2);
  }
  MSG("%s",tmp);

  if (argc != expect_argc) 
    ERR("argc == %i, expected %i", argc, expect_argc);
  else {
    for (i=1; i<argc; i++) {
      if (strcmp(argv[i], expect_args[i-1]))
        ERR("argv[%i] == '%s', expected '%s'", i, argv[i], expect_args[i-1]);
    }
  }

  BARRIER();

  MSG("done.");

  gasnet_exit(test_errs);
  return test_errs;
}
