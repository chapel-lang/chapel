const int ROWGROUPSIZE = 1024*1024*512;

#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(int);
  
  void writeParquet(int);

  void doRead(void*, int);

  void readParquet(void*, int);

#ifdef __cplusplus
}
#endif
