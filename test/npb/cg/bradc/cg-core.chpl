proc conjGrad(A: [?MatDom], X: [?VectDom]) {
  const cgitmax = 25;

  var Z: [Vectdom] = 0.0,
      R = X,
      P = R;
  var rho = sum reduce R**2;
 
  for cgit in 1..cgitmax {
    const Q = sum reduce(dim=2) [(i,j) in MatDom] (A(i,j) * P(j));

    const alpha = rho / sum reduce (P*Q);
    Z += alpha*P;
    R -= alpha*Q;

    var rho0 = rho;
    rho = sum reduce R**2;
    var beta = rho / rho0;
    P = R + beta*P;
  }
  R = sum reduce(dim=2) [(i,j) in MatDom] (A(i,j) * Z(j));
  const rnorm = sqrt(sum reduce (X-R)**2);

  return (Z, rnorm);
}
