var A = [1, 2, 3];
var B = A: [1..3] int(32);

check(A);
check(B);

proc check(arr) {
  writeln(arr, ": [", arr.domain, "] ", arr.eltType:string);
}
