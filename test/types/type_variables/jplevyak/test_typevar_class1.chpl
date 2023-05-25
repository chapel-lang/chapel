class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var f : borrowed foo(int) = (new owned foo(t=int)).borrow();

f.x = 2;

f.print();
