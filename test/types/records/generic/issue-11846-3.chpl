proc main() {
  var r: R(2, 3);
  writeln(r.type:string);
  writeln(r);
}

record R {
  param n, m: int;

  type data_t = n*(m*int);

  var data1: R(2, 3).data_t;
  var data2:         data_t;
}
