// if-object-1.chpl generates errors "applying postfix-! to nil"
// if-object-2.chpl compiles successfully

class C {
  var x: int;
}

proc main() {
  var obj: borrowed C?;

  writeln(obj!.x);    // error

  if obj then
    writeln(obj!.x);  // ok

  if obj != nil then
    writeln(obj!.x);  // ok
}
