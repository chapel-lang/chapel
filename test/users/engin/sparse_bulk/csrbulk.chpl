use LayoutCSR;
use Random;
use Sort;
use Time;

config param numTrials = 16;
config const N = 2048;
config const nnzGen = 1024;

const D = {0..#N, 0..#N};
var SD: sparse subdomain(D) dmapped new dmap(new CSR());

const r = new RandomStream(seed=5);

var inds: [{0..#nnzGen}] 2*int;


config const bulk = false;
const time = new Timer();
var times: [0..#numTrials] real;

for t in 0..#numTrials {
  time.start();
  for i in inds do i = ((r.getNext()*N):int, (r.getNext()*N):int);
  if bulk {
    SD.bulkAdd(inds);
  }
  else {
    for i in inds {
      SD += i;
    }
  }
  time.stop();
  times[t] = time.elapsed();
  time.clear();
}

for i in SD do writeln(i);

for t in times do writeln(t);

