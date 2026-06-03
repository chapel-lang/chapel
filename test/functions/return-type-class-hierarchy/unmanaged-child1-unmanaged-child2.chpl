// Test return type inference with owned and borrowed class hierarchy
use Classes;


proc test(flag: int, lhs, rhs) {
  if flag == 0 then
    return lhs;
  else
    return rhs;
}

proc main() {
  var lhs = new unmanaged Child1(1, 2);
  var rhs = new unmanaged Child2(10, 20);
  defer { delete lhs; delete rhs; }
  var r = test(1, lhs, rhs);
  writeln(r.type:string);
  writeln(r.x);
}
