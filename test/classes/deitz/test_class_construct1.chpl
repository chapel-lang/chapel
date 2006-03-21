class foo {
  var x : int = 12;
  fun initialize() {
    x += 10;
  }
}

var f : foo = foo(50);

writeln(f);
