proc foo(type t) {
  writeln("type is: ", t:string);
}

foo([1..2] real);
foo([2..3] [3..4] real);
foo([0..1, 1..2] real);
foo([1..2, 2..3] [2..3, 3..4] real);
foo([0..1, 1..2, 2..3] [4..5, 5..6] real);
