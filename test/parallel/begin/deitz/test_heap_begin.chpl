use Time;

foo();

def foo() {
  var x = 17;
  begin {
    sleep(1);
    writeln(x);
  }
  x = 18;
}
