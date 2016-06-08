
config const b = 3;
config const n = 99;

var data: [1..n] int = [i in 1..n] i % b + 1;

type HistT = [1..b] int;
var histo: HistT;

writeln("computing the histogram over ", data.domain, " with ", b, " buckets");

forall d in data with (+ reduce histo) {
  histo[d] += 1;
}

writeln("histogram = ", histo);

// just for kicks - reduce on domains

var dosto: domain(int);
writeln("playing with an associative domain");
forall d in data with (+ reduce dosto) {
  dosto.add(d);
}
forall d in data {
  if !dosto.member(d) then
    halt("dosto does not include ", d);
}
writeln("dosto is fine");
