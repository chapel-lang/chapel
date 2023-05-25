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

var f1 : borrowed foo(t=int)    = (new owned foo(t = int)).borrow();

f1.x.y = 12;

f1.print();

var f2 : borrowed foo(t=string) = (new owned foo(t = string)).borrow();

f2.x.y = "hello world";

f2.print();
