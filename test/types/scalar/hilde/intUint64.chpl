// intUint64.chpl
//
// Want this to be legal, but generate a run-time error if result does not fit.
// This means we must specify a winner for coercions at compile time: int or uint.
var i: int = 0;
var ui64: uint(64) = 3184;

writeln((i+ui64).type:string);
