class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f1 = new foo(int);
var f2 = new foo(string);

f1.x = 2;
f2.x = "hello world";

f1.print();
f2.print();

delete f2;
delete f1;
