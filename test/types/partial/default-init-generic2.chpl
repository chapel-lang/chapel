record R {
  type T;
  param n : int;
  var data : n*T;

  proc init(type T = int, param n : int = 7) {
    this.T = T;
    this.n = n;
  }
}

proc main() {
  var B : R(real);
  writeln(B.type:string, ": ", B);

  var C : R(n=3);
  writeln(C.type:string, ": ", C);
}
