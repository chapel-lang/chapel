// Test return type inference with owned and borrowed class hierarchy
use Classes;

var globalChild1 = new Child1(1, 2);

proc test(flag: int, other) {
  if flag == 0 then
    return globalChild1;
  else
    return other;
}

proc main() {
  var other = new unmanaged Child2(10, 20);
  defer { delete other; }
  var r = test(1, other);
  writeln(r.type:string);
  writeln(r.x);
}
