export proc takesCstring(s: c_string) {
  writeln(createStringWithNewBuffer(s));
}


export proc returnsTuple() {
  return (1,2);
}
