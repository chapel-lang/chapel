class C {
  var u: int(64);
  var v: int(64);
  var w: int(64);
  var x: int(64);
  var y: int(64);
  var z: int(64);
}

var c = new C();

writeln("Shouldn't get here if --memmax is less than 52");
