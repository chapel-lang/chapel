
proc modifyBlankArgArrInferRef(tup: (?t,)) where isArrayType(t) {
  tup[0][0] = 64;
}

// actual=const tuple/formal=ref
proc test1() {
  var arr: [0..0] int;
  const tup = (arr,);
  writeln(tup);
  // Referential tuple will be "coerced" out of 'tup'.
  modifyBlankArgArrInferRef(tup);
  writeln(tup);
}
test1();

// actual=referential containing const/formal=ref
proc test2() {
  const arr: [0..0] int;
  writeln(arr);
  modifyBlankArgArrInferRef((arr,));
  writeln(arr);
}
test2();

// actual=blank formal/formal=ref
proc test3() {
  var arr: [0..0] int;
  const tup = (arr,);

  proc modifyViaFormal(tup) {
    // Tuple is in the correct form. Automatically pass it off.
    modifyBlankArgArrInferRef(tup);
  }

  modifyViaFormal(tup);
  writeln(tup);
}
test3();

