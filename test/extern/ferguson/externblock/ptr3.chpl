extern {
  #include <stdio.h>

  void printit(int *numptr);
  void printit(int *numptr)
  {
    printf("num is %i\n", *numptr);
  }
}

var num:c_int = 17;
printit(c_ptrTo(num));

