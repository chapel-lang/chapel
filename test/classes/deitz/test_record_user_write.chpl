record foo {
  var x : int = 1;
  var y : int = 2;
  var z : int = 3;
}

proc foo.writeThis(fp) {
  fp.write("(---", x, " ", y, " ", z, "---)");
}

var f : foo;

writeln(f);
