proc acceptIntRef(ref arg: int) {
  arg = 7;
}

proc accept2(ref arg:int(8)) {
  acceptIntRef(arg);
}

proc test() {
  var x:int(8) = 100;

  acceptIntRef(x);

  writeln(x);
}

test();
