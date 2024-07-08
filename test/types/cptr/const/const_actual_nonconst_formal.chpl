use CTypes;

extern {
  static int foo(int* x) {
    (*x)++;
    return *x;
  }
}

var x : c_int = 5;
var xp : c_ptrConst(c_int) = c_ptrToConst(x);
writeln(foo(xp));
