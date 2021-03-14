var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}



proc foo(X: [] int) {
   ref Y = X;
   Y[1] = 42;
}


foo(getter(globalArray));
writeln(globalArray);
