#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(int, int);
  void writeParquet(int, int);

  void doRead(void*, int);
  void readParquet(void*, int);

  int c_doSize(char*);
  int cpp_getSize(char*);

#ifdef __cplusplus
}
#endif
