//Check if reduce intent forall loop is vectorizable
proc loop(A) {
  var sum : int(32) = 0;
  forall a in A with (+ reduce sum) {
    // CHECK: <4 x i32>
    sum += a;
  }
  return sum;
}

config const n = 50000;

var A:[0..n] int(32);

for i in 0..n
{
  A[i] = i : int(32);
}

var res = loop(A);
// CHECK: res = 1250025000
writeln("res = ", res);
