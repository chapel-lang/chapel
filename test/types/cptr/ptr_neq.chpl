proc test(x) {
  writeln(x != nil);
  writeln(nil != x);
}
var y: c_void_ptr = c_nil;
test(y);
