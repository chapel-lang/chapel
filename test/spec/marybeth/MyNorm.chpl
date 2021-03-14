module MyNorm{
  proc norm(x: [], p: string) where x.rank == 1 {

    if (p == '2') {
      return sqrt(+ reduce (x*x));
    } else if (p == '1') {
      return + reduce abs(x);
    } else if ((p == 'inf') || (p == 'INF') || (p == 'Inf')) {
      return max reduce abs(x);
    } else return -1;
  }

  proc norm(x:[?D], p: string) where x.rank == 2 {
  
    if (p == '1') {
      var maxColSum = abs(x(1,1));
      for j in D.dim(1) {
        maxColSum = max(maxColSum,+ reduce abs(x[D.dim(0),j]));
      }
      return maxColSum;
    } else if ((p == 'inf') || (p == 'INF') || (p == 'Inf')) {
      var maxRowSum = abs(x(1,1));
      for i in D.dim(0) {
        maxRowSum = max(maxRowSum,+ reduce abs(x[i,D.dim(1)]));
      }
      return maxRowSum;
    } else if ((p == 'frob') || (p == 'FROB') || (p == 'Frob')) {
      return sqrt(+ reduce abs(x));
    } else return -1;
  }

  proc norm(x: []) {
    if (x.rank == 1) then return norm(x,'2');
    else if (x.rank == 2) then return norm(x,'frob');
    else return -1;
  }
}

