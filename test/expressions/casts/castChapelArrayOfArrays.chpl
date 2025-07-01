// skip a case that's leaking for now;
// see test/types/arrays/arrOfArr/leaking.chpl
config param skipLeaking = true;

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

if !skipLeaking {
  var E: [1..2, 1..2] [1..3] real = (([1.1, 1.2, 1.3],
                                      [2.1, 2.2, 2.3]),
                                     ([3.1, 3.2, 3.3],
                                      [4.1, 4.2, 4.3]));
  var F = E: [0..1, 3..4] [0..5 by 2] real;

  check(E);
  check(F);
}

proc check(arr) {
  checkhelp(arr);
  writeln(": [", arr.domain, "] ", arr.eltType:string);
  writeln();
}

proc checkhelp(arr) {
  if !isArrayType(arr.eltType) then
    write(arr);
  else {
    for a in arr {
      checkhelp(a);
      writeln();
    }
  }
}
