
record R {
  type T;
  type U;
  param x : int;
}

proc makeR(type X, type T, type U, param x : int) {
  return new X(T, U, x);
}

proc makeRI(type X, type U, param x : int) {
  return new X(U, x);
}

proc makeRIS(type X, param x : int) {
  return new X(x);
}

proc testAliases() {
  type RI = R(int);
  type RIS = RI(string);
  type RIS5 = RIS(5);

  writeln("RI = ", RI:string);
  writeln("RIS = ", RIS:string);
  writeln("RIS5 = ", RIS5:string);

  var r : RIS5;
  writeln(r.type:string);
  writeln(r);

  var x = new RIS(5);
  writeln(x.type:string);
  writeln(x);
}

proc testFunctions() {
  var A = makeR(R, int, string, 5);
  writeln(A.type:string);
  writeln(A);

  var B = makeRI(R(int), string, 5);
  writeln(B.type:string);
  writeln(B);

  var C = makeRIS(R(int, string), 5);
  writeln(C.type:string);
  writeln(C);
}

proc main() {
  testAliases();
  testFunctions();
}
