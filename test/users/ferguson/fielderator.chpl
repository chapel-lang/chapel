record R {
  /* 1 */ param param_value = 1;
  /* 2 */ type type_value = int;
  /* 3 */ var x: int;
  /* 4 */ var y: string;
  /* 5 */ var z: real;
  proc something() {
    return z;
  }
}

var m = new R(1, int, 3, "hi", 17.23);
const epsilon = 1e-8; // for comparing reals.

assert(__primitive("num fields", R) == 5);

assert(__primitive("field num to name", R, 3) == "x");
assert(__primitive("field num to name", R, 4) == "y");
assert(__primitive("field num to name", R, 5) == "z");

assert(isParam(__primitive("field by num", m, 1)));
assert(isType(__primitive("field by num", m, 2)));
assert(__primitive("field by num", m, 3) == 3);
assert(__primitive("field by num", m, 4) == "hi");
assert(realEqual(__primitive("field by num", m, 5), 17.23));

for param i in 1..(__primitive("num fields", R)) {
  if isType(__primitive("field by num", m, i)) {
    writeln("type ", __primitive("field num to name", R, i));
  } else if isParam(__primitive("field by num", m, i)) {
    writeln("param ", __primitive("field num to name", R, i));
  } else {
    writeln(__primitive("field num to name", R, i));
    writeln(__primitive("field by num", m, i));
  }
}

proc realEqual(a: real, b: real): bool {
  var diff = abs(a - b);
  return diff < epsilon;
}
