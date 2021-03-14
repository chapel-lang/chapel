proc f(arg:3*real) {
  writeln(arg);
}

proc test() {
  f( (1.0, 2.0, -1) );
}

test();
