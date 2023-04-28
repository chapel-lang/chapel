extern {
  #include <stdio.h>

  static void* cfun(void) { return (void*)"Hello Void*"; }
  static void cfun2(void* s) { printf("%s\n", (char*)s); }
}

var x = cfun();
cfun2(x);


