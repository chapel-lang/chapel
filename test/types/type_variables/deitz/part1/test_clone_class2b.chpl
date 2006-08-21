class foo {
  var x;
  def print() {
    writeln(x);
  }
}

var f = foo(x = 2);

f.print();

var f2 = foo(x = 3.2);

f2.print();
