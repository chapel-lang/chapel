class Foo {
  type eltType;
  var sum: int;

  proc init(type t, args...) {
    eltType = t;
    var total = 0;
    for i in args {
      total += i;
    }
    sum = total;
  }
}

var foo = new Foo(int, 1, 2, 3, 4); // 10
writeln(foo);

var foo2 = new Foo(real, 5, 6, 7); // 18
writeln(foo2);

delete foo;
delete foo2;
