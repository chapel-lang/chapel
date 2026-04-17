proc foo(type t) {
  writeln(t.domain);
}

proc bar(idk: [] ?t) {
  writeln(t.domain);
}

foo([1..10] int);
foo([1..3] real);
foo([1..5] [1..10] bool);

var A: [1..5] [1..10] bool;
bar(A);
