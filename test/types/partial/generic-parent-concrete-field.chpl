
class C {
  type t;
  var n: int;
  var dom = {1..n};
}

class D: C {
  var A: [dom] real;
}

type D1 = D(int);
var A = new D1(10);
writeln(A.type:string, ": ", A);

type D2 = D(t=int);
var B = new D2(10);
writeln(B.type:string, ": ", B);

