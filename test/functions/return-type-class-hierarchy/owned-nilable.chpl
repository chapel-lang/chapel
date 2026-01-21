// Test return type inference with nilable owned class hierarchy
use Classes;

proc test(flag: int) {
  if flag == 0 then
    return new Child1(1, 2);
  else
    return nil:owned Child2?;
}

proc main() {
  var r1 = test(0);
  writeln(r1.type:string);
  writeln(r1!.x);

  var r2 = test(1);
  writeln(r2.type:string);
  if r2 == nil then
    writeln("nil");
}
