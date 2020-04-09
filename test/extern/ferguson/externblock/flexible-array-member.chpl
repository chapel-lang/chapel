extern {
  #include <stdlib.h>

  typedef struct {
    double zero;
    double dat[];
  } mycomplex;

  static mycomplex* new_complex(void) {
    mycomplex* ptr;
    ptr = malloc(3*sizeof(double));
    ptr->zero = 0.0;
    ptr->dat[0] = 1.0;
    ptr->dat[1] = 2.0;
    return ptr;
  }

  static void free_complex(mycomplex* ptr) {
    free(ptr);
  }
}

var x = new_complex();
writeln(x.deref().zero);
writeln(x.deref().dat[0]);
writeln(x.deref().dat[1]);

free_complex(x);
