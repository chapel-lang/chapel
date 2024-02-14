use Time, Collectives, summary;

config const k = 0.4,
             dt = 1.0,
             dx = 1.0;

config const nx = 1_000_000,
             nt = 100,
             nthreads = here.maxTaskPar;

const alp = k*dt/(dx*dx),
      tx = nx/nthreads;

var data1, data2: [-1..nx] real;
var b = new barrier(nthreads);

proc main() {
  var t = new stopwatch();

  t.start();
  forall i in -1..nx do data1[i] = 1 + (i + nx) % nx;
  coforall tid in 0..<nthreads do work(tid);
  t.stop();

  summarize(t, data1);
}

proc work(tid: int) {
  const lo = tx*tid,
        hi = min(tx*(tid+1), nx);

  for 1..nt {
    foreach i in lo..<hi do
      data2[i] = data1[i] + alp*(data1[i+1] + data1[i-1] - 2*data1[i]);

    if tid == 0 {
      data2[-1] = data2[nx-1];
      data2[nx] = data2[0];
    }

    b.barrier();

    if tid == 0 then
      data1 <=> data2;
  }
}
