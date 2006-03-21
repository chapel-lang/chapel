class foo {
  var x : int;
  fun setx(i : int) {
    x = i;
  }
  fun getx() : int {
    return x;
  }
}

var f : foo = foo();

f.setx(3);
writeln(f.getx());
