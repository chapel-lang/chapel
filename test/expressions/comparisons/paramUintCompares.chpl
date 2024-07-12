// inspired by the (unmerged) PR #5277

type intTypes = (int(8), int(16), int(32), int(64));
type uintTypes = (uint(8), uint(16), uint(32), uint(64));
type integralTypes = (int(8), int(16), int(32), int(64),
                      uint(8), uint(16), uint(32), uint(64));

for param uintIdx in 0..3 do
  for param intIdx in 0..3 do
    for param integralIdx in 0..7 do
      testit(uintTypes(uintIdx), intTypes(intIdx), integralTypes(integralIdx));

// no need to compile further
compilerError("Everything's OK!");

proc testit(type tUint, type tMinusOne, type tZero) {
  compilerWarning(tUint:string, " ", tMinusOne:string, " ", tZero:string);

  var x: tUint;
  param minusOne = (-1): tMinusOne;
  param zero = 0: tZero;

  compilerAssert((x < zero    ) == false);
  compilerAssert((minusOne < x) == true);
  compilerAssert((zero < minusOne) == false);
  compilerAssert((minusOne < zero) == true);
}
