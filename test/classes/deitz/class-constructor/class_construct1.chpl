class foo {
  var x : int = 12;
  proc postinit() {
    x += 10;
  }
}

var f : unmanaged foo = new unmanaged foo(50);

writeln(f);

delete f;
