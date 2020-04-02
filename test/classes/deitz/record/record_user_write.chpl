record foo {
  var x : int = 1;
  var y : int = 2;
  var z : int = 3;
}

proc foo.writeThis(fp) throws {
  fp.write("(---", x, " ", y, " ", z, "---)");
}

var f : foo;

writeln(f);
