// if-object-1.chpl generates errors "applying postfix-! to nil"
// if-object-2.chpl compiles successfully

class C {
  var x: int;
}

proc test(type CT) {
  var obj: CT?;

  if obj == nil then
    writeln(obj.type:string);

  writeln(obj!.x);    // error

  if obj then
    writeln(obj!.x);  // never executed

  if obj != nil then
    writeln(obj!.x);  // never executed
}

proc main {
  test(owned C);
  test(shared C);
  test(borrowed C);
  test(unmanaged C);

  writeln("done");
}
