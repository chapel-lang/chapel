extern record rec {
  var x: int;
}

export proc foo(arg: rec) {
  writeln(arg.x);
}
