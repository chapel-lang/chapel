class C {
  fun =foo(i : int, f : float) {
    writeln(i);
    writeln(f);
  }
}

var c = C();
var i = 2;
var j = 3;
c.foo(i) = j;
