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

  const char* c_getType(char* filename, char* colname);
  const char* cpp_getType(char* filename, char* colname);

  void cpp_writeColumnToParquet(char* filename, void* chpl_arr,
                                int colnum, char* dsetname, int numelems,
                                int rowGroupSize);
  void c_writeColumnToParquet(char* filename, void* chpl_arr,
                              int colnum, char* dsetname, int numelems,
                              int rowGroupSize);
  
  void c_lowLevelRead(char* filename, void* chpl_arr, int numElems);
  int cpp_lowLevelRead(char* filename, void* chpl_arr, int numElems);
    
    
 
  
#ifdef __cplusplus
}
#endif
