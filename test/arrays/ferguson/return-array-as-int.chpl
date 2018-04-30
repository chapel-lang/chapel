// From issue #8392

record NamedMatrix {
  var rowNames:[1..10] int;
}

proc NamedMatrix.setRowNames(rn:[]):int {
  return this.rowNames;
}

var a:NamedMatrix;
a.setRowNames(["bla"]);
