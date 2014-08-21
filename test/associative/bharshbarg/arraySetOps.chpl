
var foo = ["a" => 1, "b" => 2, "c" => 3, "d" => 4];
var bar = ["c" => 4, "d" => 4, "e" => 5, "f" => 6];

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

write("foo = ");
pretty(foo);
write("bar = ");
pretty(bar);
writeln();

// Union
write("Union: foo | bar = \n\t");
pretty(foo | bar);

write("Intersection: foo & bar = \n\t");
pretty(foo & bar);

write("Difference: foo - bar = \n\t");
pretty(foo - bar);

write("Xor: foo ^ bar = \n\t");
pretty(foo ^ bar);
