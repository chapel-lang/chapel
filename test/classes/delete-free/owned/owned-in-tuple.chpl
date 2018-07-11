use OwnedObject;
class Foo {
  var x: int;
};

type Bar = (Owned(Foo), int); // <-- Error here?

proc test() {
  var x:Bar;
  x(1) = new Owned(new Foo(1));
  x(2) = 2;

  writeln(x(1).borrow());
  writeln(x(2));
}

test();
