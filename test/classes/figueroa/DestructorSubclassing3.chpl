pragma "use default init"
class C1 {
  var x: int;
  proc deinit () {writeln("Inside ~C1");}
}

pragma "use default init"
class C2 {
  proc deinit () {writeln("Inside ~C2");}
}

pragma "use default init"
class C: C1, C2 {
  proc deinit () {writeln("Inside ~C");}
}

pragma "use default init"
class D {
  var y: int;
  proc deinit () {writeln("Inside ~D");}
}

pragma "use default init"
class E: C, D {
  proc deinit () {writeln("Inside ~E");}
}

var e = new E();
delete e;
