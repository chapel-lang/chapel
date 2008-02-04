class foo {
  type t;
  var x : t;
  def print() {
    writeln(x);
  }
}

var f : foo(int) = new foo(t=int);

f.x = 2;

f.print();
