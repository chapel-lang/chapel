class foo {
  type t1;
  type t2;

  var x : t1;
  var y : t2;

  proc print() {
    writeln(x, " and ", y);
  }
}

var ownF1 = new owned foo(t1 = int, t2 = string);
var f1 : borrowed foo(t1=int,t2=string) = ownF1.borrow();
var ownF2 = new owned foo(t1 = string, t2 = int, x = "hello", y = 12);
var f2 : borrowed foo(t1=string,t2=int) = ownF2.borrow();

f1.x = 14;
f1.y = "world";

f1.print();
f2.print();
