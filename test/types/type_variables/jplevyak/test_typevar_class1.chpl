class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f : foo(int) = new foo(t=int);

f.x = 2;

f.print();
