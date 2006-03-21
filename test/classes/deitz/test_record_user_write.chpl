record foo {
  var x : int = 1;
  var y : int = 2;
  var z : int = 3;
}

function fwrite(fp : file, f : foo) {
  fwrite(fp, "(---", f.x, " ", f.y, " ", f.z, "---)");
}

var f : foo;

writeln(f);
