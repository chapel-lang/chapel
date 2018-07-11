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

var f : unmanaged foo(t=int) = new unmanaged foo(t = int);

f.x.y = 12;

f.print();

delete f;
