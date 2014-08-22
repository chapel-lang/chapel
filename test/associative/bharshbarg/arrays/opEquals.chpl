
proc pretty(a : []) {
  write("{ ");
  var first = true;
  for k in a.domain.sorted() {
    if !first then write(", ", k, " => ", a[k]);
    else {
      write(k, " => ", a[k]);
      first = false;
    }
  }
  writeln(" }");
}

var a = ["a" => 1, "b" => 2, "c" => 3];
pretty(a);
var b = ["b" => 111, "d" => 4];
a += b;
pretty(a);
a -= b;
pretty(a);
a ^= b;
pretty(a);
a &= b;
pretty(a);
