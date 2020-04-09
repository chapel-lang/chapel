// This is a variant on bar() in returnGeneric.chpl
// that returns either a C or a D.
// When #11460 is resolved and this code works,
// merge it into returnGeneric.chpl.

class C {
  var n;
}

class D: C {
  var m;
}

config const flag = false;

proc baz(): unmanaged C {
 if flag then
  return new unmanaged C(3);
 else
  return new unmanaged D(4,5.5);
}

var baaz = baz();
writeln(baaz);
delete baaz;

// TODO: also test the case of iter+yield instead of proc+return.
