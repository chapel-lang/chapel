config const n = 8;

const ProbDom = {1..n, 1..n},
      BigDom = {0..n+1, 0..n+1},
      DnsStencDom = {-1..1, -1..1},
      SpsStencDom: sparse subdomain(DnsStencDom) 
                 = ( (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0) );

var A: [BigDom] real,
    B: [ProbDom] real,
    W: [ProbDom] [SpsStencDom] real;

forall (i,j) in ProbDom with (ref A, ref W) {
  A(i,j) = (i-1)*n + j;
  forall (x,y) in SpsStencDom with (ref W) {
    W(i,j)(x,y) = 1.0;
  }
}

writeln("A is:\n", A, "\n");

forall ij in ProbDom with (ref B) do
  B(ij) = (+ reduce [off in SpsStencDom] W(ij)(off)*A(ij+off)) / 6;

writeln("B is:\n", B, "\n");
