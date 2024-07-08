use LinkedLists;

class C {
  var x: int;
  proc foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  override proc foo() { writeln("D: ", this); }
}

var a = new C(1);
var b = new D(2,3);
var c = new C(4);
var d = new D(5,6);

var s: LinkedList(borrowed C) = makeList(a.borrow(), b.borrow(),
                                         c.borrow(), d.borrow());

writeln(s);

for e in s do e.foo();

s.destroy();
