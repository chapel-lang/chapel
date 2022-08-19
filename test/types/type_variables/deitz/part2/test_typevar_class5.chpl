class foo {
  type t;

  var x : t;
  var y : t;

  proc print() {
    writeln(x, " and ", y);
  }
}

var f1 : borrowed foo(t=int)    = (new owned foo(t = int)).borrow();
var f2 : borrowed foo(t=string) = (new owned foo(t = string, x = "hello", y = "world")).borrow();

f1.x = 12;
f1.y = 14;

f1.print();
f2.print();
