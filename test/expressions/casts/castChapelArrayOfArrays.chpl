var A = [[1.1, 1.2, 1.3],
         [2.1, 2.2, 2.3],
         [3.1, 3.2, 3.3]];

var B = A: [1..3] [1..3] real(32);

check(A);
check(B);

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
