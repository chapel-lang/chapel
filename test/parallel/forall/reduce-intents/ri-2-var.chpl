// Test of forall with reduce intents over a var iterator.

var global1, global2: int;

iter myiter() {
  writeln("Serial");
  yield 111;
}

iter myiter(param tag) ref where tag == iterKind.standalone {
  writeln("Standalone");
  cobegin with (ref global1, ref global2) {
    yield global1;
    yield global2;
  }
}

proc main {
  var xxx: int;
  global1 = 33;
  global2 = 500;
  forall iii in myiter() with (+ reduce xxx) {
    xxx = iii;
    iii += 5;
  }
  writeln(xxx);
  writeln(global1);
  writeln(global2);
}
