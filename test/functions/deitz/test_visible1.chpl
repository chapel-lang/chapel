class C {
  var x : int;
}

var c = new C();

def foo() {
  def bar(c : C) {
    writeln("in bar");
  }
  writeln("in foo");
}

bar(c);
