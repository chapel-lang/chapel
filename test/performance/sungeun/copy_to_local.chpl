config const n = 10;
config param printOutput = true;
config const printTiming = false;
config param doCommDiag = false;
config param debug = false;

use CyclicDist;
use Time;

const Dcyclic: domain(1) dmapped Cyclic(0) = 0..#numLocales;
const D = {1..n};

var A: [Dcyclic] [D] int(64);

forall a in A do
  a = here.id;

if printOutput then writeln(A);

var st = getCurrentTime();
coforall loc in Locales do on loc {
  const l = (here.id+1)%numLocales;
  var myA: [D] int(64);
  ref Aalias = A[l];
  if doCommDiag then startCommDiagnostics();
  myA = Aalias;
  if doCommDiag then stopCommDiagnostics();
  if (debug) {
    writeln(here, ": copying from locale ", l);
    writeln(here, ": ", + reduce [a in myA] a != l, " errors");
  }
  if doCommDiag {
    var Diagnostics = getCommDiagnostics();
    writeln(loc, " : (gets, puts, forks, fast forks, non-blocking forks)");
    for (lid, diagnostics) in (1..,Diagnostics) do
      writeln(loc, " ", lid, ": ", diagnostics);
  }
  if printOutput then writeln(loc, ":\n", myA);
 }
var dt = getCurrentTime()-st;

if printTiming then writeln("Copy to local (", numLocales, " locales): ", dt);
