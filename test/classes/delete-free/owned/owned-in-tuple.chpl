
class Foo {
  var x: int;
};

type Bar = (owned Foo?, int); // <-- Error here?

proc test() {
  var x:Bar;
  x(1) = new owned Foo(1);
  x(2) = 2;

  writeln(x(1).borrow());
  writeln(x(2));
}

test();
