proc *(A:[?D1],B:[?D2]) {
  var x = A(D1.low)*B(D2.low);
  var C:[D1.dim(0),D2.dim(1)] x.type;

  if (D1.dim(1).size != D2.dim(0).size) then
    halt("Matrix multiplication with incompatible matrices");

  for (i,j,k1,k2) in MMIterator(D1, D2) {
    C(i,j) += A(i,k1)*B(k2,j);
  }

  iter MMIterator(D1,D2) {
    for j in D2.dim(1) do 
      for (k1,k2) in zip(D1.dim(1),D2.dim(0)) do 
        for i in D1.dim(0) do 
          yield (i,j,k1,k2);
  }
  return C;
}

