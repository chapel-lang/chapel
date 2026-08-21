const n = 100, stride = 20;
var T: [0..n+1, 0..n+1] real;
var Tnew: [1..n,1..n] real;
var x, y: real;
for (i,j) in {1..n,1..n} { // serial iteration
  x = ((i:real)-0.5)/n;
  y = ((j:real)-0.5)/n;
  T[i,j] = exp(-((x-0.5)**2 + (y-0.5)**2)/0.01); // narrow Gaussian peak
}

coforall (i,j) in {1..n,1..n} by (stride,stride) { // 5x5 decomposition into 
                                                   // 20x20 blocks => 25 tasks
  for k in i..i+stride-1 { // serial loop inside each block
    for l in j..j+stride-1 do {
      Tnew[k,l] = (T[k-1,l] + T[k+1,l] + T[k,l-1] + T[k,l+1]) / 4;
    }
  }
}
