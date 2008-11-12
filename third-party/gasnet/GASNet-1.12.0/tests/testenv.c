/*   $Source: /var/local/cvs/gasnet/tests/testenv.c,v $
 *     $Date: 2007/01/10 10:43:06 $
 * $Revision: 1.4 $
 * Description: GASNet environment variable propagation test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

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

  GASNET_Safe(gasnet_init(&argc, &argv));
    startup_val = gasnet_getenv(TEST_VAR);
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
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

  sprintf(tmp, "argc=%i, argv[] = ", argc);
  for (i=0; i < argc; i++) {
    char tmp2[255];
    sprintf(tmp2, "%s'%s'", (i>0?", ":""), argv[i]);
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
