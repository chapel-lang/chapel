#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

//
// First stab at unifying aprun functionality
//
// TODO:
// - restructure to enable actual unification
// - add init routine that does the cnselect (and -cc flag stuff)
//

// FIX ME WHEN WE SORT OUT THIS BATCH SCHEDULER + LAUNCHER STUFF
// THIS SHOULD GO INTO SOME HEADER FILE
typedef enum {
  aprun_cc,   // binding policy
  aprun_n,    // cores per locale
  aprun_d,    // num locales
  aprun_N,    // locales per node
  aprun_j,    // cpus per node (newer versions of aprun)
  aprun_none
} aprun_arg_t;

void initAprunAttributes(void);
const char* getCoresPerLocaleStr(void);
int getCoresPerLocale(void);
const char* getLocalesPerNodeStr(void);
int getLocalesPerNode(void);
const char* getCPUsPerCUStr(void);
int getCPUsPerCU(void);
const char* getNumLocalesStr(void);
const char* getAprunArgStr(aprun_arg_t arg); // possibly inline
int getAprunArg(aprun_arg_t argt);           // possibly inline

//
// FIX ME ABOVE
//

#define CNAbuflen 2048
static char CNA[CNAbuflen];

static char const *aprun_arg_strings[aprun_none] = { "-cc",
                                                     "-n",
                                                     "-d",
                                                     "-N",
                                                     "-j"};

//
// Return the appropriate integer value for given argument type
//
const char* getAprunArgStr(aprun_arg_t argt) {
  if (argt < aprun_none) return aprun_arg_strings[argt];
  else return NULL;
}

int getAprunArg(aprun_arg_t argt) {
  switch (argt) {
  case aprun_cc:
    return -1; // string arg/user provides this
  case aprun_n:
    return -1; // user provides this
  case aprun_d:
    return getCoresPerLocale();
  case aprun_N:
    return getLocalesPerNode();
  case aprun_j:
    return getCPUsPerCU();
  default:
    return -1;
  }
}

//
// This function retrieves the list of attributes available from cnselect
//
void initAprunAttributes() {
  char* argv[3];
  argv[0] = (char *) "cnselect";
  argv[1] = (char *) "-l";
  argv[2] = NULL;
  
  memset(CNA, 0, CNAbuflen);
  // We assume here that 'cnselect -l' will always return something meaningful
  if (chpl_run_utility1K("cnselect", argv, CNA, CNAbuflen) <= 0) {
    chpl_error("Error trying to run 'cnselect'", 0, 0);
  }

}

const char* getNumLocalesStr() {
  return getAprunArgStr(aprun_n);
}

const char* getCoresPerLocaleStr() {
  return getAprunArgStr(aprun_d);
}
int getCoresPerLocale() {
  int numCores = -1;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores <= 0)
      chpl_warning("CHPL_LAUNCHER_CORES_PER_LOCALE set to invalid value.", 0, 0);
  }

  if (numCores > 0)
    return numCores;

  if (strstr(CNA, "numcores") != NULL) {
    const int buflen = 1024;
    char buf[buflen];
    char* argv[3];

    argv[0] = (char *) "cnselect";
    argv[1] = (char *) "-Lnumcores";
    argv[2] = NULL;
  
    memset(buf, 0, buflen);
    if (chpl_run_utility1K("cnselect", argv, buf, buflen) <= 0)
      chpl_error("Error trying to determine number of cores per node", 0, 0);

    if (sscanf(buf, "%d", &numCores) != 1)
      chpl_error("unable to determine number of cores per locale; "
                 "please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);

    return numCores;
  }

  if (strstr(CNA, "coremask") != NULL) {
    const int buflen = 1024;
    char buf[buflen];
    char* argv[3];
 
    argv[0] = (char *) "cnselect";
    argv[1] = (char *) "-Lcoremask";
    argv[2] = NULL;
  
    memset(buf, 0, buflen);
    if (chpl_run_utility1K("cnselect", argv, buf, buflen) <= 0)
      chpl_error("Error trying to determine number coremask on node", 0, 0);

    {
      int coreMask;
      int bitMask = 0x1;

      if (sscanf(buf, "%d", &coreMask) != 1)
        chpl_error("unable to determine coremask for locale; "
                   "please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);

      coreMask >>= 1;
      numCores = 1;
      while (coreMask & bitMask) {
        coreMask >>= 1;
        numCores += 1;
      }
    }

    return numCores;
  }

  // neither numcores nor coremask is available in this version
  chpl_error("Error trying to determine number of cores per node", 0, 0);

  return 0;
}

const char* getLocalesPerNodeStr() {
  return getAprunArgStr(aprun_N);
}
int getLocalesPerNode() {
  return 1;
}

const char* getCPUsPerCUStr() {
  return getAprunArgStr(aprun_j);
}
int getCPUsPerCU() {
  int numCPUsPerCU = -1;
  char* numCPUsPerCUString = getenv("CHPL_LAUNCHER_CPUS_PER_CU");

  if (numCPUsPerCUString) {
    numCPUsPerCU = atoi(numCPUsPerCUString);
    if (numCPUsPerCU < 0)
      chpl_warning("CHPL_LAUNCHER_CPUS_PER_CU set to invalid value; "
                   "using 0 instead.", 0, 0);
  }

  if (numCPUsPerCU < 0 && strstr(CNA, "cpus_per_cu") != NULL)
    numCPUsPerCU = 0;

  return numCPUsPerCU;
}
