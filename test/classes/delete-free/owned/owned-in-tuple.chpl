
class Foo {
  var x: int;
};

type Bar = (owned Foo?, int); // <-- Error here?

proc test() {
  var x:Bar;
  x(0) = new owned Foo(1);
  x(1) = 2;

  writeln(x(0).borrow());
  writeln(x(1));
}

test();
