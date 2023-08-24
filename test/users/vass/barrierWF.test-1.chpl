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
var ownB = new owned BarrierWF(tk);
const b = ownB.borrow();

// use it
coforall task in 1..tk with (ref log) {
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
