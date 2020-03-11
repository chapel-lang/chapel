/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// This file is used by both the aprun and pbs-aprun launcher code
//
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
// We need to specify the path to aprun-utils.c since this file is
// also compiled in the pbs-aprun directory
//
#include "../aprun/aprun-utils.h"

//
// First stab at unifying aprun functionality
//
// TODO:
// - add init routine that does the cnselect (and -cc flag stuff)
//


#define CNAbuflen 2048
static char CNA[CNAbuflen];

static char const *aprun_arg_strings[aprun_none] = { "-cc",
                                                     "-n",
                                                     "-d",
                                                     "-N",
                                                     "-j",
                                                     "-L"};

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

const char *getNodeListStr() {
  return getAprunArgStr(aprun_L);
}
char *getNodeListOpt() {
  const char *nodeList = getenv("CHPL_LAUNCHER_NODELIST");
  char *nodeListOpt = NULL;

  if (nodeList) {
    nodeListOpt = chpl_mem_alloc(strlen(getNodeListStr())+strlen(nodeList)+1,
                                 CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    strcpy(nodeListOpt, getNodeListStr());
    strcat(nodeListOpt, nodeList);
  }

  return nodeListOpt;
}

//
// This function allocates and returns a NULL terminated argument list
// with the aprun command to be run
//
static char _nbuf[16];
static char _dbuf[16];
static char _Nbuf[16];
static char _jbuf[16];
char** chpl_create_aprun_cmd(int argc, char* argv[],
                             int32_t numLocales, const char* _ccArg) {
  char *largv[9];  // Count the number of largv[largc++] below and adjust this
  int largc = 0;
  const char *ccArg = _ccArg ? _ccArg : "none";
  int CPUsPerCU;
  char *nodeListOpt;

  initAprunAttributes();

  largv[largc++] = (char *) "aprun";
  if (verbosity < 2) {
    largv[largc++] = (char *) "-q";
  }
  sprintf(_nbuf, "%s%d", getNumLocalesStr(), numLocales);
  largv[largc++] = (char *) getAprunArgStr(aprun_cc);
  largv[largc++] = (char *) ccArg;
  sprintf(_dbuf, "%s%d", getCoresPerLocaleStr(), getCoresPerLocale());
  largv[largc++] = _dbuf;
  largv[largc++] = _nbuf;
  sprintf(_Nbuf, "%s%d", getLocalesPerNodeStr(), getLocalesPerNode());
  largv[largc++] = _Nbuf;
  if ((CPUsPerCU = getCPUsPerCU()) >= 0) {
    sprintf(_jbuf, "%s%d", getCPUsPerCUStr(), getCPUsPerCU());
    largv[largc++] = _jbuf;
  }
  if ((nodeListOpt = getNodeListOpt()) != NULL) {
    largv[largc++] = nodeListOpt;
  }

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}
