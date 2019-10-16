// see also test/classes/vass/if-object-2.chpl

class C {
  var x: int;
}

proc main() {
  var obj: borrowed C?;

  writeln(obj.x);

  if obj then
    writeln(obj.x);

  if obj != nil then
    writeln(obj.x);
}
