class bar {
  type t;

  var  y : t;
}

class foo {
  type t;
  var  x : unmanaged bar(t) = new unmanaged bar(t);

  proc deinit() {
    delete x;
  }

  proc print() {
    writeln(x.y);
  }
}

var ownF1 = new owned foo(t = int);
var f1 : borrowed foo(t=int)    = ownF1.borrow();

f1.x.y = 12;

f1.print();

var ownF2 = new owned foo(t = string);
var f2 : borrowed foo(t=string) = ownF2.borrow();

f2.x.y = "hello world";

f2.print();
