config const n = 4;
 
def main
{
  const mat_dom : domain (2) = [1..n, 1..n];
  var A : [mat_dom] real,
      L : [mat_dom] real;

  traditional_outer_product_cholesky ( A, L );
}
 
def traditional_outer_product_cholesky ( A : [], L : [] )
{
  assert ( A.domain.dim (1) == A.domain.dim (2)  &&
           L.domain == A.domain );
  writeln("traditional_outer_product_cholesky OK");
}
