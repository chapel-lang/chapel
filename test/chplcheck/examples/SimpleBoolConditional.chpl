/*
  Conditionals that result in a single boolean value do not need to be written
  as a conditional expression. The following demonstrates this.
  The expressions for ``A`` and ``B`` are equivalent.
*/
config const x = 1;
const A = x == 1;
const B = if x == 1 then true else false;

/*
  This is also true for if statements which return a boolean value on both
  branches. The procedures ``foo`` and ``bar`` are equivalent.
*/
proc foo(x) {
  if x == 1 then
    return true;
  else
    return false;
}
proc bar(x) {
  return x == 1;
}
