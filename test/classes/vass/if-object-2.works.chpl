// see also test/classes/nilability/if-object-1.chpl

class C {
  var x: int;
}

proc test(type CT) {
  var obj: CT?;

  if obj then
    writeln(obj!.x);

  if obj != nil then
    writeln(obj!.x);
}

proc main {
  test(owned C);
  test(shared C);
  test(borrowed C);
  //test(unmanaged C); // tested in if-object2.chpl

  writeln("done");
}
