use Time;
use Math;
use CommDiagnostics;

inline proc prefetch(ref x, len:int) {
  __primitive("chpl_comm_remote_prefetch", x.locale.id,
              x, len);
}

config const n = 3000;
config const m = 10000000;
config const w = 1;
config const n_experiments = 4;
config const n_prefetch = 0;
config const verbose = false;

const M = 1000000;
const Mi = 1024*1024;

var A:[0..#m] int;

for i in 0..#m {
  A[i] = i;
}

proc f(i: int)
{
  var r = 2918*i*i*i*i + 111*i*i + 572*i + 767;

  r %= (m-w);
  if r < 0 then r += (m-w);

  return r;
}

proc test(n_prefetch:int): (int,real) {
  var ret_time : real;
  var ret_sum : int;

  on Locales[1] {
    var clock : Timer;
    var sum = 0;
    var elapsed_time = INFINITY;


    for i in 1..n_experiments {
      clock.clear();
      clock.start();

      for j in 1..n {
        var i = f(j);

        if n_prefetch > 0 {
          prefetch(A[f(j+n_prefetch)], w);
        }
        
        for k in 0..#w {
          sum += A[i+k];
        }
      }

      clock.stop();

      elapsed_time = min(elapsed_time, clock.elapsed());
    }
    ret_time = elapsed_time;
    ret_sum = sum;
  }
  return (ret_sum, ret_time);
}

resetCommDiagnostics();
startCommDiagnostics();
var (sum0,time_no_prefetch) = test(0);
stopCommDiagnostics();
var no_prefetch_diags = getCommDiagnostics();

resetCommDiagnostics();
startCommDiagnostics();
var (sum1,time_some_prefetch) = test(8);
stopCommDiagnostics();
var some_prefetch_diags = getCommDiagnostics();

assert(sum0 == sum1);

if verbose {
  writeln("No prefetch diags: ", no_prefetch_diags);
  writeln("No prefetch time: ", time_no_prefetch);
  writeln();
  writeln("Some prefetch diags: ", some_prefetch_diags);
  writeln("Some prefetch time: ", time_some_prefetch);
}

// check that prefetching helps
assert(no_prefetch_diags[1].cache_get_hits <
       some_prefetch_diags[1].cache_get_hits);
assert(no_prefetch_diags[1].cache_get_misses >
       some_prefetch_diags[1].cache_get_misses);

// check that there aren't too many misses in the prefetching configuration
assert(some_prefetch_diags[1].cache_get_misses < 300);
