
proc modifyBlankArgArr(arr) where isArrayType(arr.type) {
  arr[0] = 64;
}

proc test() {
  const arr: [0..0] int;
  writeln(arr);
  modifyBlankArgArr(arr);
  writeln(arr);
}
test();

