use LinkedLists;

class C {
  var x: int;
  proc foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  override proc foo() { writeln("D: ", this); }
}

var s: LinkedList(borrowed C) = makeList( (new owned C(1)).borrow(), (new owned D(2,3)).borrow(), (new owned C(4)).borrow(), (new owned D(5,6)).borrow() );

writeln(s);

for e in s do e.foo();

s.destroy();
