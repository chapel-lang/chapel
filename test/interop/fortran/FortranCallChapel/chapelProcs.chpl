var chplInt: int;
var chplReal: real;

// Note: These exported functions need to be all lowercase
export proc chpl_library_init_ftn() {
  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));
  var filename = c"fake";
  chpl_library_init(0, c_ptrTo(filename): c_ptr(c_ptr(c_char)));;
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
