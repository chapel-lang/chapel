
proc modifyBlankArgArrInferRef(tup: (?t,)) where isArrayType(t) {
  tup[0][0] = 64;
}

//actual=const/formal=ref
proc test1() {
  const arr: [0..0] int;
  var tup = (arr,);
  writeln(tup);
  // Referential tuple will be "coerced" out of 'tup'.
  modifyBlankArgArrInferRef(tup);
  writeln(tup);
}
test1();

proc test2() {
  const arr: [0..0] int;
  writeln(arr);
  modifyBlankArgArrInferRef((arr,));
  writeln(arr);
}
test2();

