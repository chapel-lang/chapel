
record R1 {
  param n, m: int;

  proc type data_t type {
    return n*(m*int);
  }

  var data: R1(n, m).data_t;
}

record R2 {
  param n, m: int;

  type data_t = n*(m*int);

  var data2: data_t;
}

proc main() {
  var r1: R1(2, 3);
  writeln(r1.type:string);
  writeln(r1);

  var r2: R2(2, 3);
  writeln(r2.type:string);
  writeln(r2);
}
