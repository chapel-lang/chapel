use MemDiagnostics;

use ChapelContextSupport;
use Iterators.SimpleOneDim;

config param hoistArray = true;

config const n = 20;
config const doVerboseMem = false;

if doVerboseMem then startVerboseMem();
forall i in simpleOneDim(n) {  // context should be coming from a new syntax
  const context = new Context();
  const vectorContext = __primitive("outer context", context);
  const localTaskContext = __primitive("outer context", vectorContext);
  const localeContext = __primitive("outer context", localTaskContext);
  const preLocaleTaskContext = __primitive("outer context", localeContext);

  var a: [0..<n] int;
  if hoistArray then // needs to be param-folded
    __primitive("hoist to context", localeContext, a);

  writeln(a[i]);
}
if doVerboseMem then stopVerboseMem();
