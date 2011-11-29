use Time;

proc foo() {
  var x: int = 17;
  bar(x);
  x=18;
  writeln("---");
  sleep(2);
  writeln("---");
}

proc bar(inout x) {
  begin {
    sleep(1);
    writeln(x);
  }
}

foo();
