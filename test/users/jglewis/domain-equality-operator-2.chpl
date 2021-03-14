config const n = 4;
 
proc main
{
  const mat_dom : domain (2) = {1..n, 1..n};
  var A : [mat_dom] real,
      L : [mat_dom] real;

  traditional_outer_product_cholesky ( A, L );
}
 
proc traditional_outer_product_cholesky ( A : [], L : [] )
{
  assert ( A.domain.dim (0) == A.domain.dim (1)  &&
           L.domain == A.domain );
  writeln("traditional_outer_product_cholesky OK");
}
