// Test return type inference with shared and borrowed class hierarchy
use Classes;

var globalChild1 = new shared Child1(1, 2);
var globalChild2 = new Child2(10, 20);

proc test(flag: int) {
  if flag == 0 then
    return globalChild1;
  else
    return globalChild2.borrow();
}

proc main() {
  var r = test(0);
  writeln(r.type:string);
  writeln(r.x);
}
