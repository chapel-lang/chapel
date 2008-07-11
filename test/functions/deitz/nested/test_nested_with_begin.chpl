var s: sync bool;

def foo() {
  var i: int = 2;

  def bar() {
    writeln(i);
  }

  begin {
    s;
    bar();
  }
}

def goo() {
  var i: int = 1;
  writeln(i);
}

foo();
goo();
s = true;
