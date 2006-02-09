class C {
  var x;
  var y;
}

function foo(c : C) {
  c.x = 2;
  c.y = 3;
}

function bar(c : C) {
  c.x = "hello";
  c.y = "world";
}

var c = C(), d = C();
foo(c);
bar(d);
writeln(c);
writeln(d);


