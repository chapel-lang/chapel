proc acceptRef(ref arg:int) {
  arg = 7;
}

proc test() {
  var x: sync int;

  x = 1;

  acceptRef(x);

  writeln(x.readXX());
}

test();
