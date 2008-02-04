class foo {
  type t;
  var x : t;
  def initialize() {
    x += 10;
  }
  def print() {
    writeln(x);
  }
}

var f : foo(t=int) = new foo(t=int, x=3);

f.print();
