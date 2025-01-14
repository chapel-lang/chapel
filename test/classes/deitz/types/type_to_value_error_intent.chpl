class C {
  var x: int;
}

var c: borrowed C?;

forall 1..10 with (var c = C()) {
}

writeln(c);
