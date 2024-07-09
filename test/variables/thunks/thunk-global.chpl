record R { var x: int; }
class C { var x: int; }

proc doSomething(input: int) {
  writeln("doing something!");
  return 42 + input;
}

proc doSomething(r: R) {
  writeln("doing something else!");
  return 42 + r.x;
}

proc doSomething(c: C) { // Yes, generic -- unspecified ownership for c.
  writeln("doing something else!");
  return 42 + c.x;
}

// Create a function to make sure that the variable is not a global.
var captureMeInt = 1;
var theThunk1 = __primitive("create thunk", doSomething(captureMeInt));
writeln("Not yet, Ferb!");
writeln(__primitive("force thunk", theThunk1));

var captureMeRec = new R(1);
var theThunk2 = __primitive("create thunk", doSomething(captureMeRec));
writeln("Not yet, Ferb!");
writeln(__primitive("force thunk", theThunk2));

var captureMeClassOwnedClass = new owned C(1);
var theThunk3 = __primitive("create thunk", doSomething(captureMeClassOwnedClass));
writeln("Not yet, Ferb!");
writeln(__primitive("force thunk", theThunk3));

var captureMeClassShared = new shared C(1);
var theThunk4 = __primitive("create thunk", doSomething(captureMeClassShared));
writeln("Not yet, Ferb!");
writeln(__primitive("force thunk", theThunk4));
