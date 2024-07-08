use CTypes;

extern {
  static int foo(const int* x) {
    return *x + 1;
  }
}

var x : c_int = 5;
var xp : c_ptr(c_int) = c_ptrTo(x);
writeln(foo(xp));
