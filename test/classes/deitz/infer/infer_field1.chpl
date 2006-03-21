class C {
  var x;
  var y;
}

fun foo(c : C) {
  c.x = 2;
  c.y = 3;
}

fun bar(c : C) {
  c.x = "hello";
  c.y = "world";
}

var c = C(), d = C();
foo(c);
bar(d);
writeln(c);
writeln(d);


