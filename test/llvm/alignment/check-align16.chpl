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

export proc struct_aligned16test_return_chapel(): aligned16test {
  var tt:aligned16test;
  tt = struct_aligned16test_return_c_____();
  return tt;
}
export proc struct_aligned16test_arg_chapel(in arg: aligned16test) {
  struct_aligned16test_arg_c_____(arg);
}

proc main() {
  var ttt:aligned16test;
  ttt = struct_aligned16test_return_c_____();
  struct_aligned16test_arg_c_____(ttt);
  ttt = struct_aligned16test_return_chapel();
  struct_aligned16test_arg_chapel(ttt);
}
