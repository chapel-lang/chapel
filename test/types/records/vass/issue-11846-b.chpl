
record R3 {
  param n, m: int;

  type data_t = n*(m*int);

  var data1: R3.data_t; // currently, an error: R3 is generic
  var data2:   data_t;
}

proc main() {
  var r3: R3(2, 3);
  writeln(r3.type:string);
  writeln(r3);
}
