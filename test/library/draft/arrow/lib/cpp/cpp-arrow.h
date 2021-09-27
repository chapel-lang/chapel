const int ROWGROUPSIZE = 1024*1024*512;

#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(int);
  void writeParquet(int);

  void doRead(void*, int);
  void readParquet(void*, int);

  int c_doSize(char*);
  int cpp_getSize(char*);

#ifdef __cplusplus
}
#endif
