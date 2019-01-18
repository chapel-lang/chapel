extern {
  typedef double mypair[2];

  static mypair myglobal;

  static void f(mypair arg) {
    arg[0] = 0.0;
    arg[1] = 1.0;
    // Note, in C it *is* possible to make
    // arg point somewhere else... at least with
    // some compilers.
  }
}

proc main() {

  writeln(myglobal[0]);
  writeln(myglobal[1]);

  myglobal[0] = 100.0;
  myglobal[1] = 101.0;
  writeln(myglobal[0]);
  writeln(myglobal[1]);

  f(myglobal);
  writeln(myglobal[0]);
  writeln(myglobal[1]);
}
