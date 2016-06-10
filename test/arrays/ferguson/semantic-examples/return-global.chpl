use myrecord;

var globalArray: [1..2] R;
proc returnGlobalArray() {
 return globalArray;
}
proc run() {
  var A = returnGlobalArray();
}
run();
