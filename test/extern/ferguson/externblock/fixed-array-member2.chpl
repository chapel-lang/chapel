extern {
  typedef struct {
    double dat[2];
  } mycomplex;

  static mycomplex* myfunc(void);

  static mycomplex* myfunc(void) {
    static mycomplex myglob;
    myglob.dat[0] = 1.0;
    myglob.dat[1] = 2.0;
    return (mycomplex*) &myglob;
  }
}

var x = myfunc();
writeln(x.deref().dat(0));
writeln(x.deref().dat(1));
