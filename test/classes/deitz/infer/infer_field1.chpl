class C {
  var x;
  var y;
}

def foo(c : C) {
  c.x = 2;
  c.y = 3;
}

def bar(c : C) {
  c.x = "hello";
  c.y = "world";
}

var c = C(0, 0), d = C("", "");
foo(c);
bar(d);
writeln(c);
writeln(d);


