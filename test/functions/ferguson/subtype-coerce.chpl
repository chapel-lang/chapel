
proc checkSubtype(type a, type b) {
  if isSubtype(a, b) == false then
    compilerError("unexpected: " + a:string + " not a subtype of " + b:string);
}

proc checkNotSubtype(type a, type b) {
  if isSubtype(a, b) then
    compilerError("unexpected: " + a:string + " subtype of " + b:string);
}

proc main() {
  // int(8) can coerce to int(32)
  checkSubtype(int(8), int(32)); 
  // uint(16) can coerce to int
  checkSubtype(uint(16), int); 
  // int can coerce to real
  checkSubtype(int, real);
  // string cannot coerce to int
  checkNotSubtype(string, int);
  // int cannot coerce to uint
  checkNotSubtype(int, uint);
}
