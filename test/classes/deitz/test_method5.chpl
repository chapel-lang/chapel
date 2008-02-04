class foo {
  var x : int;
  def setx(i : int) {
    x = i;
  }
  def getx() : int {
    return x;
  }
}

var f : foo = new foo();

f.setx(3);
writeln(f.getx());
