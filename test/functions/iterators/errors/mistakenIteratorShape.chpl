record BenchRecord {
  var D: domain(1);
  var n: [D] int(64);
  proc init()
  {
    this.D = {0..1};
    this.n = [0,1];
  }
}

var n = [100, 500, 100];
var results: [0..#1] BenchRecord;

for j in 0..n.size {
  // XXX: This was intended to be results[0].n[j]
  var tmp = results.n[0][j]: string;
}
