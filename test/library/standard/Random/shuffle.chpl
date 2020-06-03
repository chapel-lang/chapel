use Random;

{
  var A = ['c', 'h', 'a', 'p', 'e', 'l'];

  shuffle(A, seed=10);

  writeln(A);
}


{
  var D = {2..8 by 2},
        A : [D] int;

  for i in D {
    A[i] = i;
  }

  shuffle(A, seed=10);

  writeln(A);
}

{
  var D = {2..9 by 2 align 1},
        A : [D] int;

  for i in D {
    A[i] = i;
  }

  shuffle(A, seed=10);

  writeln(A);
}

{
  var D = {2..8 by -2},
        A : [D] int;

  for i in D {
    A[i] = i;
  }

  shuffle(A, seed=20);

  writeln(A);
}
