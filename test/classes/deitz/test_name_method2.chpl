class C {
  var x : int;
  var y : int;
  def foo(i : int, j : int) {
    x = i;
    y = j;
  }
}

var c = new C();
c.foo(j=3,i=2);
writeln(c);
