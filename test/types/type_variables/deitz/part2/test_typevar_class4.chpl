class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f1 : borrowed foo(t=int)    = (new owned foo(t=int)).borrow();
var f2 : borrowed foo(t=string) = (new owned foo(t=string, x = "hello world")).borrow();

f1.x = 2;

f1.print();
f2.print();
