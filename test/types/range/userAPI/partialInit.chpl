
proc main() {
  // Ensure that we can initialize a range constrained by a partial type
  // expression
  type SR = range(stridable=true, ?);

  var A : SR = 1..10;
  writeln(A.type:string, ": ", A);

  var B : SR = 1:uint .. 10:uint;
  writeln(B.type:string, ": ", B);
}

