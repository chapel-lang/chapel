// Test return type inference with owned and borrowed class hierarchy
use Classes;

var globalChild1 = new owned Child1(1, 2);
var globalChild2 = new owned Child2(10, 20);

proc test(flag: int) {
  if flag == 0 then
    return globalChild1.borrow();
  else
    return globalChild2.borrow();
}

proc main() {
  var r = test(1);
  writeln(r.type:string);
  writeln(r.x);
}
