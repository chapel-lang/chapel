proc bar(type t, value) {
  var Dom: t;
  var A: [Dom] real;
  writeln(A);
  Dom = value;
  writeln(A);
}

bar(domain(2), {1..3, 1..4});
bar(domain(int), (2, 4, 8));
