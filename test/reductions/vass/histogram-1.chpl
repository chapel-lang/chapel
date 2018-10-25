
config const b = 3;
config const n = 99;
config const nr=11;

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
  if !dosto.contains(d) then
    halt("dosto does not include ", d);
}
writeln("dosto is fine");

// more kicks - ensure it works for simple records

record RR {
  var xx: int;
}
proc +(a1:RR, a2:RR) {
  return new RR(xx=a1.xx+a2.xx);
}

var rtotal: RR;
writeln("playing with a record");

forall i in 1..nr with (+ reduce rtotal) {
  rtotal.xx += 10**i;
}
writeln("rtotal = ", rtotal);
