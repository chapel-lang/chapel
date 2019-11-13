export proc takesCstring(s: c_string) {
  writeln(string.createWithNewBuffer(s));
}


export proc returnsTuple() {
  return (1,2);
}
