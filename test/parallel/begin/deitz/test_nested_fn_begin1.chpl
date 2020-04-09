use Time;

proc foo() {
  var x: int = 2;
  proc bar() {
    begin {
      sleep(1);
      writeln(x);
    }
  }
  bar();
}

foo();
