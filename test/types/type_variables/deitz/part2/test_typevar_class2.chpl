class foo {
  type t;

  var x : t;

  proc print() {
    writeln(x);
  }
}

var f1 : borrowed foo(t=int)    = (new owned foo(t=int)).borrow();
var f2 : borrowed foo(t=string) = (new owned foo(t=string)).borrow();

f1.x = 2;
f2.x = "hello world";

f1.print();
f2.print();
