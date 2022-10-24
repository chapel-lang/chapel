extern {
  extern void foo();
  void bar() {
    foo();
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
