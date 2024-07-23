use Random, Time;

config const perf = false;
config const trials = if perf then 50_000_000 else 100;

proc main() {
  var rndStream = new randomStream(uint(8), seed=314159265);
  var t: stopwatch; t.start();
  var a: uint;
  for 1..trials do
    a += rndStream.next();
  t.stop();

  if perf then
    writef("time=%dr\n", t.elapsed());
  else
    writeln(a);
}
