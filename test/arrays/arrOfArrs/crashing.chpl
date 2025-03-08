proc foo(type t) {
  writeln("type is: ", t:string);
}

foo([0..1] [0..1, 1..2] real);
foo([1..2] [1..2, 2..3] [2..3] real);
