use LinkedLists;

class C {
  var x: int;
  proc foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  override proc foo() { writeln("D: ", this); }
}

var s: LinkedList(borrowed C) = makeList( new borrowed C(1), new borrowed D(2,3), new borrowed C(4), new borrowed D(5,6) );

writeln(s);

for e in s do e.foo();

s.destroy();
