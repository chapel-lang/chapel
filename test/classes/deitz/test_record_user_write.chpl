record foo {
  var x : integer = 1;
  var y : integer = 2;
  var z : integer = 3;
}

function write(f : foo) {
  write("(---", f.x, " ", f.y, " ", f.z, "---)");
}

var f : foo;

writeln(f);
