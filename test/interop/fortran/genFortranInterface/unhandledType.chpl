export proc takesCstring(s: c_string) {
  writeln(string.createWithNewBuffer(s));
}
