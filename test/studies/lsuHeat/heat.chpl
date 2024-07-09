use Time, summary;

config const k = 0.4,
             dt = 1.0,
             dx = 1.0;

config const nx = 1_000_000,
             nt = 100;

const NX : int = nx + 1;

var data1, data2: [0..NX] real;

proc update(const ref d : [] real, ref d2 : []real) {
  forall i in 1..NX-1 with (ref d2) do
    d2[i] = d[i] + dt*k/(dx*dx)*(d[i+1] + d[i-1] - 2*d[i]);
  d2[0] = d2[NX-1];
  d2[NX] = d2[1];
}

proc main() {
  var t = new stopwatch();

  t.start();
  forall i in 0..nx do data1[i] = 1 + (i + nx) % nx;
  for t in 1..nt do {
    update(data1, data2);
    data1 <=> data2;
  }
  t.stop();

  summarize(t, data1);
}
