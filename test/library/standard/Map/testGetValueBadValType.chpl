use Map;

proc test() {
  var m: map(string, int);

  m.add("one", 1);
  m.add("two", 2);

  var x = m.getValue("one");
  x = 128;

  writeln(m);
}
test();

