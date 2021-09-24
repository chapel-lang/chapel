const int NUMVALS = 100000;
const int ROWGROUPSIZE = 1000;

#ifdef __cplusplus
extern "C" {
#endif

  int foo(int);
  
  int callFoo(int);

  void doWrite(void);
  
  void writeParquet(void);

#ifdef __cplusplus
}
#endif
