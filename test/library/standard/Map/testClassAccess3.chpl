use Map;

class C {
  var i: int;
}

proc test() {
  var m: map(string, owned C);

  m.add("one", new owned C(1));
  m.add("two", new owned C(2));

  var x = m["one"]!.borrow();
  x.i = -1;

  writeln(m);
}
test();

