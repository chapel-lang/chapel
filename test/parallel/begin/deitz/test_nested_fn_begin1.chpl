use Time;

def foo() {
  var x: int = 2;
  def bar() {
    begin {
      sleep(1);
      writeln(x);
    }
  }
  bar();
}

foo();
