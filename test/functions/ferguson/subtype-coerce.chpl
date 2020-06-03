
proc checkCoercible(type a, type b) {
  if isCoercible(a, b) == false then
    compilerError("unexpected: " + a:string + " not a subtype of " + b:string);
}

proc checkNotCoercible(type a, type b) {
  if isCoercible(a, b) then
    compilerError("unexpected: " + a:string + " subtype of " + b:string);
}

proc main() {
  // int(8) can coerce to int(32)
  checkCoercible(int(8), int(32));
  // uint(16) can coerce to int
  checkCoercible(uint(16), int);
  // int can coerce to real
  checkCoercible(int, real);
  // string cannot coerce to int
  checkNotCoercible(string, int);
  // int cannot coerce to uint
  checkNotCoercible(int, uint);
}
