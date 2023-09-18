class foo {
  var x;
  proc print() {
    writeln(x);
  }
}
var fObj = new foo(2);
var f : borrowed foo(2) = fObj.borrow();

f.print();
