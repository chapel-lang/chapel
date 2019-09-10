
record R {
  type T;
  param n : int;
  var data : n*T;

  proc init(type T = int, param n : int = 7) {
    this.T = T;
    this.n = n;
  }
}

record S {
  type T;
  var x : T;

  proc init() {
    this.T = int;
  }
}

proc main() {
  var x : range(?);
  writeln(x.type:string, ": ", x);

  var A : R;
  writeln(A.type:string, ": ", A);
  var B : R(real);
  writeln(B.type:string, ": ", B);
  var C : R(n=3);
  writeln(C.type:string, ": ", C);

  var D : S;
  writeln(D.type:string, ": ", D);
}
