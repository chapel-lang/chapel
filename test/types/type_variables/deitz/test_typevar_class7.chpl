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

var f1 : foo(t=integer) = foo(t = integer);
var b1 : bar(t=integer) = bar(t = integer);
var f2 : foo(t=string) = foo(t = string);
var b2 : bar(t=string) = bar(t = string);

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
