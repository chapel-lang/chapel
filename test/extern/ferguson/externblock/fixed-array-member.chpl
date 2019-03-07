extern {
  typedef struct {
    double dat[2];
  } mycomplex;

  static mycomplex myfunc(void) {
    mycomplex x;
    x.dat[0] = 0.0;
    x.dat[1] = 1.0;
    return x;
  }
}

proc main() {
  var x = myfunc();
  writeln(x.dat[0]);
  writeln(x.dat[1]);
  //writeln(x); prints out pointer currently

  var y = x;
  writeln(y.dat[0]);
  writeln(y.dat[1]);

  var z: mycomplex;
  z = x;
  writeln(z.dat[0]);
  writeln(z.dat[1]);
}
