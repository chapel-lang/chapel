use Map;

class C { var x: int = 0; }

proc test() {
  var m: map(string, owned C);

  m.add("one", new owned C(1));
  m.add("two", new owned C(2));

  var x = m.getValue("one");
  x = 128;

  writeln(m);
}
test();

