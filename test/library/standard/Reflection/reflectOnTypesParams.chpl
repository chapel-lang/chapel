use Reflection;

record R {
  type t = real;
  param p = 1;
  var a: t;
}

var r = new R();

for param i in 0..<numFields(r.type) {
  if isParam(getField(r, i)) {
    param f = getField(r,i);
    writeln("field ", i, " = ", f, " is a param");
  } else if isType(getField(r, i)) {
    type f = getField(r,i);
    writeln("field ", i, " = ", f:string, " is a type");
  } else {
    var f = getField(r,i);
    writeln("field ", i, " = ", f, " is not a param or type");
  }
}

for param i in 0..<numFields(r.type) {
  param fName = getFieldName(R, i);
  if isParam(getField(r, fName)) {
    param f = getField(r, fName);
    writeln("field ", fName, " = ", f, " is a param");
  } else if isType(getField(r, fName)) {
    type f = getField(r,i);
    writeln("field ", fName, " = ", f:string, " is a type");
  } else {
    var f = getField(r,i);
    writeln("field ", fName, " = ", f, " is not a param or type");
  }
}
