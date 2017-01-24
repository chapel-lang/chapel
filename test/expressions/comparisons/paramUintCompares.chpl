//
// These uint comparisons should all be compile-time folded false
//
proc testit(type t) {
  var x: t = 2;

  if (x < 0) then
    compilerError("This should always be false");

  if (!(x >= 0)) then
    compilerError("This should always be false too");

  if (0 > x) then
    compilerError("As should this");

  if (!(0 <= x)) then
    compilerError("And also this");
}

//
// Test comparisons for all uints
//
testit(uint);
testit(uint(64));
testit(uint(32));
testit(uint(16));
testit(uint(8));

//
// This isn't actually an error, but avoids paying any execution time
//
compilerError("Everything's OK!");
