bar1();
proc bar1() {
  proc foo(i: int) {
    writeln("outermost foo");
  }
  bar2();
  proc bar2() {
    proc foo(i: int) {
      writeln("middle foo");
    }
    bar3();
    proc bar3() {
      proc foo(r: real) {
        writeln("innermost foo, but worst match");
      }
      var x:int = 1;
      foo(x);
    }
  }
}
