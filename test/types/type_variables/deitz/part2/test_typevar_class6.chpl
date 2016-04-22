class foo {
  type t1;
  type t2;

  var x : t1;
  var y : t2;

  proc print() {
    writeln(x, " and ", y);
  }
}

var f1 : foo(t1=int,t2=string) = new foo(t1 = int, t2 = string);
var f2 : foo(t1=string,t2=int) = new foo(t1 = string, t2 = int, x = "hello", y = 12);

f1.x = 14;
f1.y = "world";

f1.print();
f2.print();

delete f2;
delete f1;
