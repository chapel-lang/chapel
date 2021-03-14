config const n = 100;
config type myType = real;

var T: [1..n] real;
[i in 1..n] T[i] = if isIntegralType(myType) then i else i*0.5;

proc foo(type myType) {
  var A = T:myType;
  var x: sync myType = 0:myType;
  forall i in 1..n do
    x.writeEF(x.readFE() + A[i]);
  var sum: myType;
  sum = + reduce A;
  if x.readXX() != sum then
    writeln("ERROR: sums do not match for ", myType:string,
            " (should be ", sum, ", result is ", x.readXX(), ")");
}

foo(int(32));
foo(int(64));
foo(uint(32));
foo(uint(64));
foo(real(32));
foo(real(64));
