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

var ownF1 = new owned foo(t = int);
var f1 : borrowed foo(t=int)    = ownF1.borrow();
var ownB1 = new owned bar(t = int);
var b1 : borrowed bar(t=int)    = ownB1.borrow();

var ownF2 = new owned foo(t = string);
var f2 : borrowed foo(t=string) = ownF2.borrow();
var ownB2 = new owned bar(t = string);
var b2 : borrowed bar(t=string) = ownB2.borrow();

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
