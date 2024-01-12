use IO;
use MemDiagnostics;

use ChapelContextSupport;
use Iterators.SimpleOneDim;

config const n = 20;
config const doVerboseMem = true;

if doVerboseMem then startVerboseMem();
forall i in simpleOneDim(n) {  // context should be coming from a new syntax
  const context = new Context();
  const vectorContext = __primitive("outer context", context);
  const localTaskContext = __primitive("outer context", vectorContext);
  const localeContext = __primitive("outer context", localTaskContext);
  const preLocaleTaskContext = __primitive("outer context", localeContext);

  serial {
    writeln(here, " %2i ".format(i), localTaskContext, " ", localeContext, " ",
            preLocaleTaskContext);
  }
}
if doVerboseMem then stopVerboseMem();
