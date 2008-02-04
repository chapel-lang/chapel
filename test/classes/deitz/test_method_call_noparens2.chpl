class C {
  var x = 2;
  def foo return 3;
  def bar return foo;
}

var c = new C(4);

writeln(c.bar);
