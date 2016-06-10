use myrecord;

proc createArray() {
 var retA: [1..2] R;
 return retA;
}
// Are the array elements copied in the
// process of moving from retA to A here?
var A = createArray();
writeln(A[1]);
