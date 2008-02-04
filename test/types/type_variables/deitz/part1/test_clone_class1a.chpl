class foo {
  var x;
  def print() {
    writeln(x);
  }
}

var f = new foo(3.2);

f.print();

f.x = 2;

f.print();
