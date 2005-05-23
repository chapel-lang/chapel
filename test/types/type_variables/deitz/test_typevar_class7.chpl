class bar {
  type t;
  var y : t;
  function print() {
    writeln(y);
  }
}

class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f1 : foo = foo(t = integer);
var b1 : bar = bar(t = integer);
var f2 : foo = foo(t = string);
var b2 : bar = bar(t = string);

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
