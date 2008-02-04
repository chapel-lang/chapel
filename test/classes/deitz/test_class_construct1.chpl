class foo {
  var x : int = 12;
  def initialize() {
    x += 10;
  }
}

var f : foo = new foo(50);

writeln(f);
