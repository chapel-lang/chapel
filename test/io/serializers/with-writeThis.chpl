
use IO;

record R {
  var x : int;

  proc writeThis(f) {
    f.write("writeThis: ", x);
  }
}

proc main() {
  var r = new R(5);
  writeln(r);
}
