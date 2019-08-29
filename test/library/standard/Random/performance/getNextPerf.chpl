use Random, Time;

config const perf = false;
config const trials = if perf then 50_000_000 else 100;

proc test(parallel, param parSafe) {
  var rndStream = new RandomStream(uint(8), seed=314159265, parSafe=parSafe);
  var t: Timer; t.start();
  var a: uint;
  if parallel then
    forall 1..trials with (+reduce a) do
      a += rndStream.getNext();
  else
    for 1..trials do
      a += rndStream.getNext();
  t.stop();

  if perf then
    writef("parallel=%t-parSafe=%t-time=%dr\n", parallel, parSafe, t.elapsed());
  else
    writeln(a);
}

test(parallel=false, parSafe=false);
test(parallel=false, parSafe=true);
test(parallel=true , parSafe=true);
