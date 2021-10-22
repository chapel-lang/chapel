#ifdef __cplusplus
extern "C" {
#endif
  
  void doWrite(int, int);
  void writeParquet(int, int);

  void doRead(void*, int);
  void readParquet(void*, int);

  int c_doSize(const char*);
  int cpp_getSize(const char*);

  void c_readColumnByIndex(const char* filename, void* chpl_arr, int colNum, int numElems);
  void cpp_readColumnByIndex(const char* filename, void* chpl_arr, int colNum, int numElems);

  void c_readColumnByName(const char* filename, void* chpl_arr, const char* colname, int numElems);
  void cpp_readColumnByName(const char* filename, void* chpl_arr, const char* colname, int numElems);

  const char* c_getType(const char* filename, const char* colname);
  const char* cpp_getType(const char* filename, const char* colname);

  void cpp_writeColumnToParquet(const char* filename, void* chpl_arr,
                                int colnum, const char* dsetname, int numelems,
                                int rowGroupSize);
  void c_writeColumnToParquet(const char* filename, void* chpl_arr,
                              int colnum, const char* dsetname, int numelems,
                              int rowGroupSize);
  
  void c_lowLevelRead(const char* filename, void* chpl_arr, const char* colname, int numElems);
  int cpp_lowLevelRead(const char* filename, void* chpl_arr,const char* colname, int numElems);
    
  const char* c_getVersionInfo(void);
  const char* cpp_getVersionInfo(void);
 
  
#ifdef __cplusplus
}
#endif
