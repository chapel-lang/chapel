class bar {
  type t;

  var  y : t;

  proc print() {
    writeln(y);
  }
}

class foo {
  type t;
  var  x : t;

  proc print() {
    writeln(x);
  }
}

var f1 : borrowed foo(t=int)    = new borrowed foo(t = int);
var b1 : borrowed bar(t=int)    = new borrowed bar(t = int);

var f2 : borrowed foo(t=string) = new borrowed foo(t = string);
var b2 : borrowed bar(t=string) = new borrowed bar(t = string);

f1.x = 12;
f2.x = "hello";
b1.y = 12;
b2.y = "world";

f1.print();
f2.print();
b1.print();
b2.print();
