class foo {
  type t;

  var x : t;

  proc print() {
    writeln(x);
  }
}

var f1 : foo(t=int)    = new foo(t=int);
var f2 : foo(t=string) = new foo(t=string);

f1.x = 2;
f2.x = "hello world";

f1.print();
f2.print();

delete f2;
delete f1;
