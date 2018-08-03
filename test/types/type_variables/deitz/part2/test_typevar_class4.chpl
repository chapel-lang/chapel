class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f1 : borrowed foo(t=int)    = new borrowed foo(t=int);
var f2 : borrowed foo(t=string) = new borrowed foo(t=string, x = "hello world");

f1.x = 2;

f1.print();
f2.print();
