use Time;

def foo() {
  var x: int = 17;
  bar(x);
}

def bar(x) {
  begin {
    sleep(1);
    writeln(x);
  }
}

foo();
writeln("---");
