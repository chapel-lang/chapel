class foo {
  var x;
  def print() {
    writeln(x);
  }
}

var f = foo("howdy");

f.print();

var f2 = foo(3.2);

f2.print();
