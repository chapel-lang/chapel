record R {
  var x: int;
}

record StoresR {
  var x: [1..3] R;
}

var storing = new StoresR();
writeln(storing);
