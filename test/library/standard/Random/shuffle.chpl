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

{
  var D = {1..4 by 2, 1..6 by 3};
  var A : [D] int;
  
  for i in 0..D.size-1 {
    var ind = D.orderToIndex(i);
    A[ind] = i+1;
  }

  shuffle(A, seed=10);

  writeln(A);
}

{
  var D = {1..3, 1..3};
  var A: [D] string;
  
  A[1,1] = 'a';
  A[1,2] = 'b';
  A[1,3] = 'c';
  A[2,1] = 'd';
  A[2,2] = 'e';
  A[2,3] = 'f';
  A[3,1] = 'g';
  A[3,2] = 'h';
  A[3,3] = 'i';

  shuffle(A, seed=10);

  writeln(A);
}