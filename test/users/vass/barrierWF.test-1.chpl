use barrierWF;

// num tasks
config const tk = 8; //here.numCores; should be maxTaskPar?

// num tries
config const ty = 1000;

const entries = tk * ty;
var log: [1..entries] int;
var cur: atomic int;

writeln("starting");

// create it
const b = new BarrierWF(tk);

// use it
coforall task in 1..tk {
  for try in 1..ty {
    log[cur.fetchAdd(1)+1] = try;
    b.barrier();
  }
}

// check it
for try in 1..ty {
  for task in 1..tk {
    const ix = (try-1) * tk + task;
    if log[ix] != try then
      writeln("try ", try, "  task ", task, "  ix ", ix, "  got ", log[ix]);
  }
}

writeln("done");
