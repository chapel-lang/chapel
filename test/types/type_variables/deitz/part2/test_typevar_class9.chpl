class bar {
  type t;

  var  y : t;
}

class foo {
  type t;
  var  x : bar(t) = new bar(t);

  proc deinit() {
    delete x;
  }

  proc print() {
    writeln(x.y);
  }
}

var f1 : foo(t=int)    = new foo(t = int);

f1.x.y = 12;

f1.print();

var f2 : foo(t=string) = new foo(t = string);

f2.x.y = "hello world";

f2.print();

delete f2;
delete f1;
