use Reflection;

proc parse(ref data: ?t) {
  param numfield = numFields(t);
  for param i in 0..<numfield {
    type vtype = if isType(getField(data, i)) then getField(data, i)
                                              else getField(data, i).type;
    writeln(getFieldName(t, i));
    if isRecordType(vtype) || isClassType(vtype) {
      parse(getFieldRef(data, i));
    }
  }
}

record Thing2 {
  var c: int;
}

record GenericThing {
  type B;
  var a: int;
  var b: B;
}

var thing = new GenericThing(Thing2);
parse(thing);

