// Test return type inference with shared class hierarchy
use Classes;

proc test(flag: int) {
  if flag == 0 then
    return new shared Child1(1, 2);
  else
    return new shared Child2(3, 4);
}

proc main() {
  var r = test(1);
  writeln(r.type:string);
  writeln(r.x);
}
