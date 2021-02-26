config const n = 2;
var b: [0..n+1, 0..n+1, 0..1] int;

proc initb() {
  for i in 0..n+1 {
    for j in 0..n+1 {
      for k in 0..1 {
        b[i,j,k] = 10000*i + 100*j + k;
      }
    }
  }
}

{
  initb();
  const N = b[0..n-1, 0..n-1, 0];
  b = 0;
  writeln("====== const N ======");
  writeln(N);
  writeln(N.type:string);
}

{
  initb();
  const NN = b[0..n-1, 0..n-1, 0] + b[0..n-1, 2..n+1, 0];
  b = 0;
  writeln("====== const NN =====");
  writeln(NN);
  writeln(NN.type:string);
}

{
  initb();
  const ref N = b[0..n-1, 0..n-1, 0];
  b = 0;
  writeln("====== const ref N ======");
  writeln(N);
  writeln(N.type:string);
}

{
  initb();
  const ref NN = b[0..n-1, 0..n-1, 0] + b[0..n-1, 2..n+1, 0];
  b = 0;
  writeln("====== const ref NN =====");
  writeln(NN);
  writeln(NN.type:string);
}
