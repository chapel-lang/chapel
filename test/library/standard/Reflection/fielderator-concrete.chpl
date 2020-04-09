use Reflection;

proc parse(ref data: ?t) {
  param numfield = numFields(t);
  for param i in 0..<numfield {
    type vtype = getField(data, i).type;
    writeln(getFieldName(t, i));
    if isRecordType(vtype) || isClassType(vtype) {
      parse(getFieldRef(data, i));
    }
  }
}

record Thing2 {
  var c: int;
}

record ConcreteThing {

  var a: int;
  var b: Thing2;
}

var thing = new ConcreteThing();
parse(thing);

