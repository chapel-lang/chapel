extern {
  #include <stdio.h>

  static void runit(double (*f)(double)) {
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

var x = c_ptrTo(f);

runit(x);
