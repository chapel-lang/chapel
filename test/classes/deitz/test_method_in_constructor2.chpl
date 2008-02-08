class C {
  type t;
  var x: t = foo();
  def foo() return 314:t;
}

var c = new C(int(64));

writeln(c);
