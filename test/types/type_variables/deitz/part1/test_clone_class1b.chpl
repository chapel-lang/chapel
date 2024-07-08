class foo {
  var x;
  proc print() {
    writeln(x);
  }
}
var fObj = new foo(2);
var f = fObj.borrow();

f.print();

f.x = 3.2;

f.print();
