class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var ownF1 = new owned foo(t=int);
var f1 : borrowed foo(t=int)    = ownF1.borrow();
var ownF2 = new owned foo(t=string, x = "hello world");
var f2 : borrowed foo(t=string) = ownF2.borrow();

f1.x = 2;

f1.print();
f2.print();
