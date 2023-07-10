extern {
  extern void foo(void);
  void bar(void) {
    foo();

    // provoke a warning for casting to smaller int type
    short x = 5;
    short* xp = &x;
    x = (int)xp;
  }
}

var globalArr : [1..1] real;

export proc foo() {
  writeln(here.id, " # before");
  writeln(here.id, " # ", globalArr.size);
  writeln(here.id, " # after");
}

proc main() {
  coforall loc in Locales do on loc {
      // this works: foo();                                                      
      bar();
  }
}
