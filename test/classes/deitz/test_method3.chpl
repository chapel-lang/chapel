class foo {
  var x : int;
  def setxlo(i : int) {
    x = i-1;
  }
  def setxhi(i : int) {
    x = i+1;
  }
}

var f : foo = new foo();

f.setxhi(3);
writeln(f.x);
f.setxlo(3);
writeln(f.x);
