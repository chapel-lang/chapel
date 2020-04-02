/////////////////////////////////
use IO;

var pfx = "# ";
var phase = 0;
var no_pfx = false;

// to preserve coarse ordering ("phases") when sorting the output
proc nextphase() {
  phase += 1;
  pfx = if no_pfx then "" else "%{####}".format(phase) + "  ";
}

// Force the next phase to be 'ph' - so that when phases are added earlier,
// downstream output does not change.
// It is OK to advance to (the current phase number + 1), or later.
proc fphase(ph) {
  if phase >= ph {
    hd("###### advancing phase to a duplicate phase number: ",
       phase, " -> ", ph);
    tl();
  }
  phase = ph - 1;
}

// heading/start new "phase"
proc hd(args...) { nextphase(); writeln(pfx, (...args)); }
proc hhd(args...) { nextphase(); writeln(pfx, (...args), " on ", here.id); }
// additional messages
proc msg(args...) { writeln(pfx, (...args)); }
proc msgp() { write(pfx); }
proc msgserial(arg) { write(pfx); for e in arg do write(" ", e); writeln(); }
// tail - finish a "phase"
proc tl()         { writeln(pfx); }

/////////////////////////////////
iter it(N:int) {
//  writeln("it serial: 1..", N);
  for i in 1..N do yield i;
}
iter it(param tag: iterKind, N:int) where tag == iterKind.leader {
  const result = 0..#N;
//  writeln("it leader: ", result);
  yield (result,);
}
iter it(param tag: iterKind, followThis, N:int) where tag == iterKind.follower {
//  writeln("it follower: ", followThis);
  if !(isTuple(followThis) && followThis.size == 1) then
    halt("it follower: argument is not a tuple or its size is not 1");
  for i in followThis(0)+1 do yield i;
}

/////////////////////////////////
proc tn(ph:int, N:int, M:int) {
  const rn = 1..N,
        d1n = {1..N},
        d2n = {1..2, 1..N};

  const rm = 1..M,
        d1m = {1..M},
        d2m = {1..2, 1..M};

  fphase(ph); hd("N = ", N, "  M = ", M); tl();

  hd("it * it"); forall i in zip(it(N), it(M)) do msg(i); tl();
  hd("it * r");  forall i in zip(it(N), rm)    do msg(i); tl();
  hd("it * d1"); forall i in zip(it(N), d1m)   do msg(i); tl();
//hd("it * d2"); forall i in zip(it(N), d2m)   do msg(i); tl();

  hd("r * it");  forall i in zip(rn, it(M)) do msg(i); tl();
  hd("r * r");   forall i in zip(rn, rm)    do msg(i); tl();
  hd("r * d1");  forall i in zip(rn, d1m)   do msg(i); tl();
//hd("r * d2");  forall i in zip(rn, d2m)   do msg(i); tl();

  hd("d1 * it"); forall i in zip(d1n, it(M)) do msg(i); tl();
  hd("d1 * r");  forall i in zip(d1n, rm)    do msg(i); tl();
  hd("d1 * d1"); forall i in zip(d1n, d1m)   do msg(i); tl();
//hd("d1 * d2"); forall i in zip(d1n, d2m)   do msg(i); tl();

//hd("d2 * it"); forall i in zip(d2n, it(M)) do msg(i); tl();
//hd("d2 * r");  forall i in zip(d2n, rm)    do msg(i); tl();
//hd("d2 * d1"); forall i in zip(d2n, d1m)   do msg(i); tl();
  hd("d2 * d2"); forall i in zip(d2n, d2m)   do msg(i); tl();
}

config const sz = 4, empty = 0;
//const er = 1..empty,
//      e1d = {1..empty},
//      e2d = {1..2, 1..empty};

proc main() {
  // hd("dataParTasksPerLocale = ", dataParTasksPerLocale); tl();

  tn(100, empty, empty);
  tn(200,    sz,    sz);
  tn(300, empty,    sz);
  tn(400,    sz, empty);  // raises an error on it() * range
}
