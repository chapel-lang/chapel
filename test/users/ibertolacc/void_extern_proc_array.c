#include "void_extern_proc_array.h"

void voidArrayFunction( int* a, int elems ){
  for( int i = 0; i < elems; i += 1 ){
    a[i] = i;
  }
}