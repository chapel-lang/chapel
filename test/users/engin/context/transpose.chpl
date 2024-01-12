use BlockDist;
use Collectives;
use CommDiagnostics;
use ChapelContextSupport;
use Time;

use Iterators.TertiaryDRDomIterators;

config type dataType = int;
config param assertLocal = false;

config const n = 8;
config const printOutput = false;
config const reportPerf = true;
config const commDiag = false;
config const verboseComm = false;
config const verify = false;

const Space = {0..<n, 0..<n};
const Dom = Block.createDomain(Space);
var Input: [Dom] int;
var Output: [Dom] int;

forall (i,j) in Dom {
  Input[i,j] = i*n+j;
}

var t: stopwatch;

if commDiag then startCommDiagnostics();
if verboseComm then startVerboseComm();
if reportPerf then t.start();

type ctx1 = Context(1, int(64));
type ctx2 = Context(2, (int(64), int(64)));

forall (i, j) in Dom.customThese() {
  const context = new Context();
  const vectorContext = __primitive("outer context", ctx1, context);
  const localTaskContext = __primitive("outer context", ctx1, vectorContext);
  const localeContext = __primitive("outer context", ctx2, localTaskContext);

  var localBarrier;
  { // AST is too complicated without this block
    localBarrier = new barrier(1);
    localBarrier.multiply(1);
  }
  __primitive("hoist to context", localeContext, localBarrier);

  var localTile;
  {
    const ref locSubDom = Dom.localSubdomain();
    localTile = Input[{locSubDom.dim(1), locSubDom.dim(0)}];
  }
  __primitive("hoist to context", localeContext, localTile);

  local assertLocal {
    Output[i,j] = localTile[j,i];
  }
}

if reportPerf {
  t.stop();
  const bytesMoved = n**2*numBytes(dataType);
  writeln("Elapsed time (s): ", t.elapsed());
  writeln("Aggregate throughput (GB/s): ", bytesMoved/t.elapsed());
  writeln("Per-locale throughput (GB/s/locale): ", bytesMoved/t.elapsed()/numLocales);
}


if verboseComm then stopVerboseComm();
if commDiag {
  stopCommDiagnostics();
  printCommDiagnosticsTable();
}

if verify {
  forall (i,j) in Dom {
    assert(Input[i,j] == Output[j,i]);
  }
}

if printOutput {
  writeln(Output);
}
