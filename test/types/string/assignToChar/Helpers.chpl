public use CTypes;
proc testInit(param strBytes, type toType) {
  var x: toType = strBytes;
  writeln("x: ", x, " ", x.type:string);
}
proc testInitVal(strBytes, type toType) {
  var x: toType = strBytes;
  writeln("x: ", x, " ", x.type:string);
}
proc testAssign(param strBytes, type toType) {
  var x: toType = 0:toType;
  x = strBytes;
  writeln("x: ", x, " ", x.type:string);
}
proc testAssignVal(strBytes, type toType) {
  var x: toType = 0:toType;
  x = strBytes;
  writeln("x: ", x, " ", x.type:string);
}
proc testCall(param strBytes, type toType) {
  proc foo(y: toType) {
    writeln("y: ", y, " ", y.type:string);
  }
  foo(strBytes);
}
proc testCallVal(strBytes, type toType) {
  proc foo(y: toType) {
    writeln("y: ", y, " ", y.type:string);
  }
  foo(strBytes);
}
proc testCArray(param strBytes, type toType) {
  use CTypes;
  var arr: c_array(toType, strBytes.size);
  for param i in 0..#strBytes.size do
    arr[i] = strBytes[i];
  writeln("arr: ", arr, " ", arr.type:string);
}
proc testCArrayVal(strBytes, type toType) {
  use CTypes;
  var arr: c_array(toType, 10);
  arr[0] = strBytes;
  writeln("arr: ", arr, " ", arr.type:string);
}

