class foo {
  type t;

  var x : t;
  var y : t;

  proc print() {
    writeln(x, " and ", y);
  }
}

var ownF1 = new owned foo(t = int);
var f1 : borrowed foo(t=int)    = ownF1.borrow();
var ownF2 = new owned foo(t = string, x = "hello", y = "world");
var f2 : borrowed foo(t=string) = ownF2.borrow();

f1.x = 12;
f1.y = 14;

f1.print();
f2.print();
