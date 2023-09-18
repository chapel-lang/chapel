extern {
  #include <stdio.h>

  typedef double (*function_ptr_type)(double x);
  static void runit(function_ptr_type f) {
     double result;
     printf("in runit, calling f(1.0)\n");
     result = f(1.0);
     printf("in runit, result is %lf\n", result);
  }
}

proc f(x:real):real {
  writeln("in f(", x, ")");
  return x + 1.1;
}

runit(c_ptrTo(f));

var x : function_ptr_type;
x = nil;
x = c_ptrTo(f);

runit(x);
