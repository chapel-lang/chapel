class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

var ownF = new owned foo(t=int);
var f : borrowed foo(int) = ownF.borrow();

f.x = 2;

f.print();
