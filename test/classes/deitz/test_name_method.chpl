class C {
  var x : int;
  def foo(i : int) {
    x = i;
  }
}

var c = C();
c.foo(i=2);
writeln(c);
