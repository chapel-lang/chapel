var A : [1..10, 1..10, 1..10] int = 0;

// contiguous slice
ref Asub = A[2..2, 4..4, ..];
ref Asubvec = reshape(Asub, 1..10);

// non-contiguous slice
ref Asub2 = A[2..5, 2..5, 2..5];
ref Asub2vec = reshape(Asub, 1..(4*4*4));
