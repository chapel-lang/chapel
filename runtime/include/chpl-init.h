#ifndef _CHPL_INIT_H_
#define _CHPL_INIT_H_

#ifndef LAUNCHER

void chpl_rt_preUserCodeHook(void);
void chpl_rt_postUserCodeHook(void);

#endif // LAUNCHER

void chpl_rt_init(int argc, char* argv[]);
void chpl_rt_finalize(int return_value);

void chpl_executable_init(void);
//   chpl_executable_finalize(...) equivalent is done by main.c:main(...)

void chpl_library_init(int argc, char* argv[]);
void chpl_library_finalize(void);

void chpl_std_module_init(void);

#endif // _CHPL_INIT_H_
