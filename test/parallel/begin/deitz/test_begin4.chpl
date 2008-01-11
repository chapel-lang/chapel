use Time;

def foo() {
  var x: int = 17;
  bar(x);
}

def bar(inout x) {
  begin {
    sleep(1);
    writeln(x);
  }
}

foo();
writeln("---");
