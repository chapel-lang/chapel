// Test return type inference with owned class hierarchy
use Classes;

proc test(flag: int) {
  if flag == 0 then
    return new Child1(1, 2);
  else
    return new Child2(3, 4);
}

proc main() {
  var r = test(0);
  writeln(r.type:string);
  writeln(r.x);
}
