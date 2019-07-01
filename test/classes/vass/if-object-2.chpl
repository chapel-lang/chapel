// see also test/classes/nilability/if-object-1.chpl

class C {
  var x: int;
}

proc main() {
  var obj: C;

  if obj then
    writeln(obj.x);

  if obj != nil then
    writeln(obj.x);

  writeln("done");
}
