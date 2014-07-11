#ifndef _APRUN_UTILS_H_
#define _APRUN_UTILS_H_

typedef enum {
  aprun_cc,   // binding policy
  aprun_n,    // num locales
  aprun_d,    // cores per locale
  aprun_N,    // locales per node
  aprun_j,    // cpus per node (newer versions of aprun)
  aprun_k,    // place on Xeon Phi co-processor
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

char** chpl_create_aprun_cmd(int argc, char* argv[],
                             int32_t numLocales, const char* _ccArg);

#endif
