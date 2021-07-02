#include "cHelper.h"

int x = 14;
uint32_t y = 3;

int baz() {
  return x;
}

int useMyType(myType arg) {
  return (int)arg;
}

int sum(int a, int b){
    return a+b;
}

int sumArray(int64_t a[], int size){
    int i;
    int retVal =0;
    for(i=0; i<size; ++i){
        retVal += a[i];
    }
    return retVal;
}