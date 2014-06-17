class foo {
  var x : int;
  proc setx(i : int) {
    x = i;
  }
  proc getx() : int {
    return x;
  }
}

var f : foo = new foo();

f.setx(3);
writeln(f.getx());
