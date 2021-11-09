proc testint() {
  writeln("testint");
  var x: sync int;

  x.writeEF(1);
  var y = x.readFE();
  assert(y == 1); // OK this makes sense

  var z = x.readXX();
  writeln(z); // What value should z have? 0? 1?
}
testint();

proc teststring() {
  writeln("teststring");
  var x: sync string;

  x.writeEF("hi"*4);
  var y = x.readFE();
  assert(y == "hi"*4); // OK this makes sense

  var z = x.readXX();
  writeln("'", z, "'"); // prints default inited value (empty string)
}
teststring();
