const int NUMVALS = 100000;
const int ROWGROUPSIZE = 1000;

#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(void);
  
  void writeParquet(void);

  void doRead(void* chpl_arr);

  void readParquet(void* chpl_arr);

#ifdef __cplusplus
}
#endif
