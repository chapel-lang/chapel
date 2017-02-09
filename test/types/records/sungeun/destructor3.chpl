record myR {
  var x: int;
  proc deinit() { writeln("bye"); }
}

var r: myR;
writeln(r);
