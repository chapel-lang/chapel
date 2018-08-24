// comparisons

config type t32 = real(32);
config type t64 = real(64);

param one32:t32 = 1.0;
param one64:t64 = 1.0;
param two32:t32 = 2.0;
param two64:t64 = 2.0;

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
