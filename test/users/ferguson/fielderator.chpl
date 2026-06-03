use Reflection;
record R {
  /* 0 */ param param_value = 1;
  /* 1 */ type type_value = int;
  /* 2 */ var x: int;
  /* 3 */ var y: string;
  /* 4 */ var z: real;
  proc something() {
    return z;
  }
}

var m = new R(1, int, 3, "hi", 17.23);
const epsilon = 1e-8; // for comparing reals.

assert(getNumFields(R) == 5);

assert(getFieldName(R, 2) == "x");
assert(getFieldName(R, 3) == "y");
assert(getFieldName(R, 4) == "z");

assert(isParam(getField(m, 0)));
assert(isType(getField(m, 1)));
assert(getField(m, 2) == 3);
assert(getField(m, 3) == "hi");
assert(realEqual(getField(m, 4), 17.23));

for param i in 0..<getNumFields(R) {
  if isType(getField(m, i)) {
    writeln("type ", getFieldName(R, i));
  } else if isParam(getField(m, i)) {
    writeln("param ", getFieldName(R, i));
  } else {
    writeln(getFieldName(R, i));
    writeln(getField(m, i));
  }
}

proc realEqual(a: real, b: real): bool {
  var diff = abs(a - b);
  return diff < epsilon;
}
