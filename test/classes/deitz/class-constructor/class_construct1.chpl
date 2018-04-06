class foo {
  var x : int = 12;
  proc postinit() {
    x += 10;
  }
}

var f : foo = new foo(50);

writeln(f);

delete f;
