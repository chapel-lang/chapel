enum color { red, green, blue};

record R {
  var x: int;
}
class C {
  var x: int;
}
union U {
  var x: int;
  var y: real;
}

config type t = bool;

param p: t;

compilerError("Passed!");  // just to save execution time
