var chplInt: int;
var chplReal: real;

export proc chpl_library_init_ftn() {
  use CTypes;

  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));

  var filenamePtr1 = "fake" : c_ptr(c_char);
  const argc: c_int = 1;
  const argv: c_ptr(c_ptr(c_char)) = c_ptrTo(filenamePtr1);

  chpl_library_init(argc, argv);
  chpl__init_chapelProcs();
}

export proc setint(i: int) {
  writeln("in setint, i = ", i);
  chplInt = i;
}

export proc setreal(r: real) {
  writeln("in setreal, r = ", r);
  chplReal = r;
}

export proc getint(): int {
  writeln("in getint, chplInt = ", chplInt);
  return chplInt;
}

export proc getreal(): real {
  writeln("in getreal, chplReal = ", chplReal);
  return chplReal;
}
