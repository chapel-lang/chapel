import IO;

extern {
  #include <stdio.h>
  #include <stdbool.h>
  #include <inttypes.h>
  #include <complex.h>
  #include <string.h>

  struct aligned16test {
    uint64_t x;
    uint64_t a __attribute__((aligned (16)));
  };
  struct aligned16test struct_aligned16test_return_c_____(void);
  void struct_aligned16test_arg_c_____(struct aligned16test arg);

  struct aligned16test struct_aligned16test_return_c_____(void) {
    struct aligned16test t;
    memset(&t, 0, sizeof(t));
    t.x = 123456789;
    t.a = 987654321;
    return t;
  }
  void struct_aligned16test_arg_c_____(struct aligned16test arg) {
    printf("arg.x %li arg.a %li\n", arg.x, arg.a);
  }
}

record R {
  var field1: aligned16test;
  var field2: int;
}

proc returnR(): R {
  var tt:R;
  tt.field1 = struct_aligned16test_return_c_____();
  tt.field2 = 1;
  return tt;
}
proc acceptR(in arg: R) {
  struct_aligned16test_arg_c_____(arg.field1);
}

proc main() {
  var ttt:R;
  ttt = returnR();
  acceptR(ttt);
  acceptR(ttt);
}
