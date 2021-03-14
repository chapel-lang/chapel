// comparisons

config type t32 = real(32);
config type t64 = real(64);

param neg32:t32 = (-1.0):t32;
param neg64:t64 = (-1.0):t64;
param one32:t32 = 1.0:t32;
param one64:t64 = 1.0:t64;
param two32:t32 = 2.0:t32;
param two64:t64 = 2.0:t64;
param four32:t32 = 4.0:t32;
param four64:t64 = 4.0:t64;

proc checkYes(param arg:bool) {
  if arg == false then
    compilerWarning("assert failed");
}
proc checkNo(param arg:bool) {
  if arg == true then
    compilerWarning("assert failed");
}

// ==
checkYes(one32 == one32);
checkYes(one64 == one64);
checkYes(one32 == one64);
checkYes(one64 == one32);
checkNo(one32 == two32);
checkNo(one64 == two64);
checkNo(one32 == two64);
checkNo(one64 == two32);

// !=
checkYes(one32 != two32);
checkYes(one64 != two64);
checkYes(one32 != two64);
checkYes(one64 != two32);
checkNo(one32 != one32);
checkNo(one64 != one64);
checkNo(one32 != one64);
checkNo(one64 != one32);

if !isComplexType(t32) && !isImagType(t32) {
  // <
  checkYes(one32 < two32);
  checkYes(one64 < two64);
  checkYes(one32 < two64);
  checkYes(one64 < two32);
  checkNo(two32 < one32);
  checkNo(two64 < one64);
  checkNo(two32 < one64);
  checkNo(two64 < one32);
  checkNo(one32 < one32);
  checkNo(one64 < one64);
  checkNo(one32 < one64);
  checkNo(one64 < one32);

  // >
  checkYes(two32 > one32);
  checkYes(two64 > one64);
  checkYes(two32 > one64);
  checkYes(two64 > one32);
  checkNo(one32 > two32);
  checkNo(one64 > two64);
  checkNo(one32 > two64);
  checkNo(one64 > two32);
  checkNo(one32 > one32);
  checkNo(one64 > one64);
  checkNo(one32 > one64);
  checkNo(one64 > one32);


  // <=
  checkYes(one32 <= two32);
  checkYes(one64 <= two64);
  checkYes(one32 <= two64);
  checkYes(one64 <= two32);
  checkNo(two32 <= one32);
  checkNo(two64 <= one64);
  checkNo(two32 <= one64);
  checkNo(two64 <= one32);
  checkYes(one32 <= one32);
  checkYes(one64 <= one64);
  checkYes(one32 <= one64);
  checkYes(one64 <= one32);

  // >=
  checkYes(two32 >= one32);
  checkYes(two64 >= one64);
  checkYes(two32 >= one64);
  checkYes(two64 >= one32);
  checkNo(one32 >= two32);
  checkNo(one64 >= two64);
  checkNo(one32 >= two64);
  checkNo(one64 >= two32);
  checkYes(one32 >= one32);
  checkYes(one64 >= one64);
  checkYes(one32 >= one64);
  checkYes(one64 >= one32);

  checkYes(min(one32, two32) == one32);
  checkYes(max(one32, two32) == two32);
  checkYes(min(one64, two64) == one64);
  checkYes(max(one64, two64) == two64);
}

// unary plus
checkYes(+one32 == one32);
checkYes(+one64 == one64);

// unary minus
checkYes(-one32 == neg32);
checkYes(-one64 == neg64);

// +
checkYes(one32 + one32 == two32);
checkYes(one64 + one64 == two64);
checkYes(one32 + one64 == two64);
checkYes(one64 + one32 == two64);

// -
checkYes(two32 - one32 == one32);
checkYes(two64 - one64 == one64);
checkYes(two32 - one64 == one64);
checkYes(two64 - one32 == one64);

if !isComplexType(t32) && !isImagType(t32) {
  // *
  checkYes(one32 * one32 == one32);
  checkYes(one64 * one64 == one64);
  checkYes(one32 * two32 == two32);
  checkYes(two64 * one64 == two64);

  checkYes(two32 * two32 == four32);
  checkYes(two64 * two64 == four64);
  checkYes(two32 * two64 == four64);
  checkYes(two64 * two32 == four64);

  // /
  checkYes(four32 / two32 == two32);
  checkYes(four64 / two64 == two64);
  checkYes(four32 / two64 == two64);
  checkYes(four64 / two32 == two64);
}
