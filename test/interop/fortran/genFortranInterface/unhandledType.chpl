export proc takesCstring(s: c_string) {
  writeln(createStringWithNewBuffer(s));
}
