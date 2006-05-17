class C {
  var x = 2;
  fun foo return 3;
  fun bar return foo;
}

var c = C(4);

writeln(c.bar);
