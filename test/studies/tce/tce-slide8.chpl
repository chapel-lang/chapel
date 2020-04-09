config const V = 3000,
             O = 100;

const DV = 1..V,
      DO = 1..O;

const DVVOO = {DV, DV, DO, DO},
      DVVVO = {DV, DV, DV, DO};

var A, C, S: [DVVOO] real,
    B, D: [DVVVO] real;

forall (a, b, i, j) in DVVOO do
  S(a,b,i,j) = + reduce [(c,d,e,f,k,l) in {DV,DV,DV,DV,DO,DO}]
                   (A(a,c,i,k) * B(b,e,f,l) * C(d,f,j,k) * D(c,d,e,l));

writeln(S);
