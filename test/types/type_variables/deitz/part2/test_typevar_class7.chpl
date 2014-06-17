class bar {
  type t;
  var y : t;
  proc print() {
    writeln(y);
  }
}

class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f1 : foo(t=int) = new foo(t = int);
var b1 : bar(t=int) = new bar(t = int);
var f2 : foo(t=string) = new foo(t = string);
var b2 : bar(t=string) = new bar(t = string);

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
