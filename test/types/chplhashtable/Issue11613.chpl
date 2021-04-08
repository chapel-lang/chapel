record R {
	type t;
	var a : [1..10] t;
}

operator R.==(a: R, b: R) {
  return && reduce (a.a == b.a);
}

operator R.!=(a: R, b: R) {
  return || reduce (a.a != b.a);
}

var d : domain(R(int));
d += new R(int);

writeln(d);
