#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(int, int);
  void writeParquet(int, int);

  void doRead(void*, int);
  void readParquet(void*, int);

  int c_doSize(char*);
  int cpp_getSize(char*);

  void c_readColumnByIndex(char* filename, void* chpl_arr, int colNum, int numElems);
  void cpp_readColumnByIndex(char* filename, void* chpl_arr, int colNum, int numElems);

  void c_readColumnByName(char* filename, void* chpl_arr, char* colname, int numElems);
  void cpp_readColumnByName(char* filename, void* chpl_arr, char* colname, int numElems);

#ifdef __cplusplus
}
#endif
