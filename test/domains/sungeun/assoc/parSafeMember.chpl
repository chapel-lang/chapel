config const iters = 800;
config const n = 100;

var D: domain(real);

for i in 1..iters {
  coforall t in 1..n {
    if n & 1 {
      for j in 1..iters do
        if !D.member((t+j):real) then D += (t+j);
    } else {
      for j in 1..iters by -1 do
        if !D.member((t+j):real) then D += (t+j);
    }
  }
  D.clear();
}
