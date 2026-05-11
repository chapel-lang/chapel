config var x: uint;

if x < 0 {
  compilerError("This should never happen");
}

if 0 > x {
  compilerError("This should never happen");
}

if x >= 0 {}
else {
  compilerError("This should never happen");
}

if 0 <= x {}
else {
  compilerError("This should never happen");
}

