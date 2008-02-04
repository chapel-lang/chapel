class foo {
  var i : int;
  def primary() {
    writeln("primary method call; i is ", i);
  }
  def setx() {
    i = 4;
  }
}

var f : foo = new foo();

f.setx();
f.primary();
