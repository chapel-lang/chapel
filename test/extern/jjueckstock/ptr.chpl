module OuterModule {
  /**************************************/
  module C { extern {
    #include "ptr.h"
    #include <stdio.h>

    struct st {
      const char* c;
      int foo;
    };

    static struct st a_st;
    static struct st *a_st_ptr = &a_st;

    static inline int crazy_ptrs(int**** x) {
      --(****x);
      return (****x)-1;
    }

    static inline void write_int_ref(int* x) {
      printf("%d\n", (*x));
    }

    static inline void change_int_ref(int* x) {
      *x += 1;
    }

    static inline int* get_int_ref(void) {
      return &foo;
    }
    
    static inline void struct_ptr_test(struct st* s) {
      printf("struct s contains %s and %d\n", s->c, s->foo);
    }

    static inline struct st* get_struct_ref(void) {
      a_st.c = "Hello";
      a_st.foo = 8;
      return a_st_ptr;
    }
  } }

  use C;
  use CPtr;

  var a: C.st;
  a.c = "a string".c_str();
  a.foo = 897234;

  var b: c_ptr(C.st) = get_struct_ref();

  struct_ptr_test(c_ptrTo(a));
  struct_ptr_test(b);

  var y = get_int_ref();
  write_int_ref(y);
  var z:c_ptr(c_ptr(c_int)) = c_ptrTo(y);
  var w = c_ptrTo(z);
  var q = c_ptrTo(w);
  writeln(crazy_ptrs(q));
  writeln(C.foo);

  var x:c_int = 999;
  change_int_ref(c_ptrTo(x));
  writeln(x);
}
