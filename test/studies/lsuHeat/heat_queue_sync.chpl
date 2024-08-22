use Time, summary;

config const k = 0.4,
             dt = 1.0,
             dx = 1.0;

config const nx = 1_000_000,
             nt = 100,
             nthreads = here.maxTaskPar;

const alp = k*dt/(dx*dx),
      tx = (2+nx)/nthreads;

var data: [0..<nx] real;

const QSIZE = 10;
var ghosts: [0..1, 0..QSIZE-1, 0..<nthreads] sync real;
param LEFT = 0, RIGHT = 1;

proc main() {
  var t = new stopwatch();

  t.start();
  coforall tid in 0..<nthreads do work(tid);
  t.stop();

  summarize(t, data);
}

proc work(tid: int) {
  const lo: int = tx*tid - 1,
        hi: int = min(tx*(tid+1),nx) + 1;

  const taskDom = {lo..<hi},
        taskDomInner = taskDom.expand(-1);

  var data1, data2: [taskDom] real;
  forall i in taskDom do data1[i] = i + 1;

  const tidP1 = (tid + 1) % nthreads,
        tidM1 = (tid + nthreads - 1) % nthreads;

  ghosts[RIGHT, 1, tidM1].writeEF(data1[taskDomInner.low]);
  ghosts[LEFT, 1, tidP1].writeEF(data1[taskDomInner.high]);

  for tl in 1..nt {
    data1[taskDom.low] = ghosts[LEFT, tl%QSIZE, tid].readFE();
    data1[taskDom.high] = ghosts[RIGHT, tl%QSIZE, tid].readFE();

    foreach i in taskDomInner do
      data2[i] = data1[i] + alp*(data1[i+1] + data1[i-1] - 2*data1[i]);

    ghosts[RIGHT, (tl+1)%QSIZE,  tidM1].writeEF(data1[taskDomInner.low]);
    ghosts[LEFT, (tl+1)%QSIZE,  tidP1].writeEF(data1[taskDomInner.high]);

    data1 <=> data2;
  }

  data[taskDomInner] = data1[taskDomInner];
}
