var A: [1..10] int;  // for forall loop

proc fun2(param arg2) {
  compilerWarning(arg2: string, 0);
  writeln("fun2 ", arg2);
}

proc fun1(param arg1) {
  forall a in A {
    compilerWarning(arg1: string, 0);
    fun2(arg1);
  }
}

proc main {
  fun1(5);
}
