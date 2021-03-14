record R {
  var x;
}
proc foo(arg:R(?t) ... ?k) {
  writeln(t:string, " ", k);
}

foo(new R(1), new R(2));
