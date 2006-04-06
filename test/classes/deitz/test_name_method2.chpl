class C {
  var x : int;
  var y : int;
  fun foo(i : int, j : int) {
    x = i;
    y = j;
  }
}

var c = C();
c.foo(j=3,i=2);
writeln(c);
