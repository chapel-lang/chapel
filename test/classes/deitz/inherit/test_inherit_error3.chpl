class C : D {
  var x : int = 1;
}

record D {
  var y : int = 1;
}

var c : C = new C();

writeln(c);
