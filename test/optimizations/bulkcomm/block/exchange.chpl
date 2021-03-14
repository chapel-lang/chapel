
use BlockDist;
use Barriers;
use CommDiagnostics;
use Time;

enum Pattern {
  Elegant,
  DestLocal,
  BothLocal
}
use Pattern;

config const n = 10000;
config const pat : Pattern = Elegant;
config const report = false;
config const time = false;

proc main() {
  var tmr : Timer;

  const Space = {0..#(numLocales*numLocales*n)};
  const D     = Space dmapped Block(Space);

  var A : [D] int;

  var b = new Barrier(numLocales);

  if !time then resetCommDiagnostics();
  else tmr.start();

  coforall loc in Locales do on loc {
    ref dstSlice = A.localSlice(A.localSubdomain());
    for off in 1..(numLocales-1) {
      const srcID = (here.id + off) % numLocales;
      const srcRange = (srcID   * numLocales * n + srcID * n)..#n;
      const dstRange = (here.id * numLocales * n + srcID * n)..#n;

      if !time then startCommDiagnosticsHere();

      if pat == Elegant {
        // Block = Block
        A[dstRange] = A[srcRange];

      } else if pat == DestLocal {
        // DR = Block
        dstSlice[dstRange] = A[srcRange];

      } else {
        assert(pat == BothLocal);
        on Locales[srcID] {
          ref srcSlice = A.localSlice(A.localSubdomain());
          const temp : [srcRange] int = srcSlice[srcRange];
          // DR = DR
          dstSlice[dstRange] = temp;
        }
      }

      if !time then stopCommDiagnosticsHere();

      b.barrier();
    }
  }

  // TODO: When we achieve the desired performance, add threshold checks for
  // correctness.
  if report {
    if time {
      writeln("Time: ", tmr.elapsed());
    } else {
      const diags = getCommDiagnostics();

      var gets : uint;
      gets += + reduce diags.get;
      gets += + reduce diags.get_nb;

      var puts : uint;
      puts += + reduce diags.put;
      puts += + reduce diags.put_nb;

      var ons : uint;
      ons += + reduce diags.execute_on;
      ons += + reduce diags.execute_on_fast;
      ons += + reduce diags.execute_on_nb;

      writeln("GETS: ", gets);
      writeln("PUTS: ", puts);
      writeln("ONS: ", ons);
    }
  }
}
