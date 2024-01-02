proc main() {
  var r: R(2, 3);
  writeln(r.type:string);
  writeln(r);
}

record R {
  param n, m: int;

  proc type data_t type {
    return n*(m*int);
  }

  var data: R(n, m).data_t;
}
