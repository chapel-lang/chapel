class bar {
  type t;

  var  y : t;
}

class foo {
  type t;
  var  x : unmanaged bar(t) = new unmanaged bar(t);

  proc deinit() {
    delete x;
  }

  proc print() {
    writeln(x.y);
  }
}

var f1 : borrowed foo(t=int)    = new borrowed foo(t = int);

f1.x.y = 12;

f1.print();

var f2 : borrowed foo(t=string) = new borrowed foo(t = string);

f2.x.y = "hello world";

f2.print();
