class C {
  var x : int;
}

var c = C();

def foo() {
  def bar(c : C) {
    writeln("in bar");
  }
  writeln("in foo");
}

bar(c);
