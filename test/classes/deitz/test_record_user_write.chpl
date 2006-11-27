record foo {
  var x : int = 1;
  var y : int = 2;
  var z : int = 3;
}

def foo.write(fp : file) {
  fp.write("(---", x, " ", y, " ", z, "---)");
}

var f : foo;

writeln(f);
