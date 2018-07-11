class C {
  var x;
  var y;
}

proc foo(c : borrowed C) {
  c.x = 2;
  c.y = 3;
}

proc bar(c : borrowed C) {
  c.x = "hello";
  c.y = "world";
}

var c = new unmanaged C(0, 0), d = new unmanaged C("", "");
foo(c);
bar(d);
writeln(c);
writeln(d);
delete c;
delete d;


