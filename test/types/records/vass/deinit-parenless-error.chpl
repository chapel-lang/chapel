
record R {
  var x: int;
  proc deinit { writeln("R.deinit"); }
}

var r: R;
