class bar {
  type t;

  var  y : t;

  proc print() {
    writeln(y);
  }
}

class foo {
  type t;
  var  x : t;

  proc print() {
    writeln(x);
  }
}

var f1 : borrowed foo(t=int)    = (new owned foo(t = int)).borrow();
var b1 : borrowed bar(t=int)    = (new owned bar(t = int)).borrow();

var f2 : borrowed foo(t=string) = (new owned foo(t = string)).borrow();
var b2 : borrowed bar(t=string) = (new owned bar(t = string)).borrow();

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
