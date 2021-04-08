record R {
	type t;
	var a : [1..10] t;
}
proc ==(a: R, b: R) {
  return && reduce (a.a == b.a);
}
proc !=(a: R, b: R) {
  return || reduce (a.a != b.a);
}

var d : domain(R(int));
d += new R(int);

writeln(d);
