var A = [[1.1, 1.2, 1.3],
         [2.1, 2.2, 2.3],
         [3.1, 3.2, 3.3]];

var B = A: [1..3] [1..3] real(32);

check(A);
check(B);

var C = [[[1.11, 1.12],
          [1.21, 1.21]],
         [[2.11, 2.12],
          [2.21, 2.22]]];

var D = C: [2..5 by 2] [1..3 by 2] [1..2] real(32);

check(C);
check(D);

proc check(arr) {
  checkhelp(arr);
  writeln(": [", arr.domain, "] ", arr.eltType:string);
  writeln();
}

proc checkhelp(arr) {
  if !isArrayType(arr.eltType) then
    writeln(arr);
  else
    for a in arr do
      checkhelp(a);
      
}
