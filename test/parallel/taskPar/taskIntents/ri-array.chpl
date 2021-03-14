// The reproducer from GitHub issue #13569

config const n = 100;
config const m = 10;
  
proc main() {
  var counts: [1..n] int;
  coforall tid in 1..m with (+ reduce counts) {
    counts[tid] += tid;
  }
  writeln(counts[1..min(n,m)]);
}
