extern {
  #include <stdlib.h>

  static int64_t* mymalloc(void) {
    int64_t *ret = (int64_t*) malloc(1024);
    ret[0] = 5;
    return ret;
  }
  static void myfree(int64_t *x) {
    free(x);
  }
}


var x = mymalloc();

writeln(x.deref());

myfree(x);

