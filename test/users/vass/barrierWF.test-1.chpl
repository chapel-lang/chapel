use barrierWF;

// num tasks
config const tk = here.maxTaskPar;

// num tries
config const ty = 1000;

const entries = tk * ty;
var log: [1..entries] int;
var cur: atomic int;

writeln("starting");

// create it
const b = new borrowed BarrierWF(tk);

// use it
coforall task in 1..tk {
  for tryN in 1..ty {
    log[cur.fetchAdd(1)+1] = tryN;
    b.barrier();
  }
}

// check it
for tryN in 1..ty {
  for task in 1..tk {
    const ix = (tryN-1) * tk + task;
    if log[ix] != tryN then
      writeln("try ", tryN, "  task ", task, "  ix ", ix, "  got ", log[ix]);
  }
}

writeln("done");
