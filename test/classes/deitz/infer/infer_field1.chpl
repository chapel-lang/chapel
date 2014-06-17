class C {
  var x;
  var y;
}

proc foo(c : C) {
  c.x = 2;
  c.y = 3;
}

proc bar(c : C) {
  c.x = "hello";
  c.y = "world";
}

var c = new C(0, 0), d = new C("", "");
foo(c);
bar(d);
writeln(c);
writeln(d);
delete c;
delete d;


