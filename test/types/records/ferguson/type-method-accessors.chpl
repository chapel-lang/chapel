record R {
  type t;
}


// 3 ways of writing R(_).t

writeln(R(int).t:string);

type MyR = R(int);
writeln(MyR.t:string);

proc subtype(type myr) type {
  return myr.t;
}

writeln(subtype(R(int)):string);

class X {
  type eltee;
  param strided;
  param rank;
}

proc printer(type T) {
  writeln(T.eltee:string);
  writeln(T.strided);
  writeln(T.rank);
}

proc main() {
  var A : X(real, false, 1);
  var B : X(uint, true, 3);

  printer(A.type);
  printer(B.type);
}
