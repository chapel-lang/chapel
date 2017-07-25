//
// 2017-07-25 Future
//
// It is not possible to make two instances of the same generic type
// when there are multiple type fields.
//

class Foo {
  type t1;
  type t2;

  var  x1: t1;
  var  x2: t2;

  proc init(type t1Val, type t2Val) {
    t1 = t1Val;
    t2 = t2Val;

    super.init();
  }
}

var foo1 = new Foo(int, int);
var foo2 = new Foo(int, int);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);

delete foo1;
delete foo2;
