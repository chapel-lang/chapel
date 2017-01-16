use myrecord;

proc createArray() {
 var retA: [1..2] R;
 return retA;
}
// Are the array elements copied in the
// process of moving from retA to A here?
// It seems clear that they need not be copied in this program.
var A = createArray();
writeln(A[1].x);

