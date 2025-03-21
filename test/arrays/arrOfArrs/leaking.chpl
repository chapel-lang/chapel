proc foo(type t) {
  writeln("type is: ", t:string);
}

foo([0..1, 1..2] [0..1] real);
foo([0..1, 1..2, 2..3] [1..2] real);
foo([1..2, 2..3] [2..3] [3..4] real);        //       twice
foo([2..3, 3..4] [3..4, 4..5] [4..5] real);
foo([4..5, 5..6] [5..6] [6..7] [7..8] real);  //       three times
