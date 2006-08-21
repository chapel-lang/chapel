class foo {
  var x;
  def print() {
    writeln(x);
  }
}

var f = foo(2);

f.print();

f.x = 3.2;

f.print();
