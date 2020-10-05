// Function yielding arrle of references to globals. Captured in a
// variable, which means the iterator is converted into an array.

record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

iter inferYieldType() ref {
  yield (g1, g2);
}

iter specifyYieldType() ref: (int, r) {
  yield (g1, g2);
}

// Return type is inferred.
proc test1() {
  writeln("test1");
  writeln("g1= " + g1:string);
  writeln("g2= " + g2:string);
  var arr = inferYieldType();
  writeln("type of arr: " + arr.type:string);
  writeln("arr= " + arr:string);
  writeln("assigning elements");
  arr[0][0] = 128;
  arr[0][1] = new r(128);
  writeln("arr= " + arr:string);
  writeln("g1= " + g1:string);
  writeln("g2= " + g2:string);
}
test1();

// Return type is specified.
proc test2() {
  writeln("test2");
  writeln("g1= " + g1:string);
  writeln("g2= " + g2:string);
  var arr = specifyYieldType();
  writeln("type of arr: " + arr.type:string);
  writeln("arr= " + arr:string);
  writeln("assigning elements");
  arr[0][0] = 128;
  arr[0][1] = new r(128);
  writeln("arr= " + arr:string);
  writeln("g1= " + g1:string);
  writeln("g2= " + g2:string);
}
test2();

