// Test that unrelated classes don't unify (even though they share Object)
use Classes;

proc test(flag: int) {
  if flag == 0 then
    return new Child1(1, 2);
  else
    return new Unrelated(3);
}

proc main() {
  var r = test(0);
  writeln(r.type:string);
}
