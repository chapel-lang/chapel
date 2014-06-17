use Time;

proc foo() {
  var x: int = 17;
  bar(x);
}

proc bar(x) {
  begin {
    sleep(1);
    writeln(x);
  }
}

foo();
writeln("---");
