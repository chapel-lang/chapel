use IO;
use Collectives;
use MemDiagnostics;

use ChapelContextSupport;
use Iterators.SimpleOneDim;

config param hoistArray = true;

config const n = 20;
config const doVerboseMem = true;

if doVerboseMem then startVerboseMem();
forall i in simpleOneDim(n) {  // context should be coming from a new syntax
  const context = new Context();
  const vectorContext = __primitive("outer context", context);
  const localTaskContext = __primitive("outer context", vectorContext);
  const localeContext = __primitive("outer context", localTaskContext);
  const preLocaleTaskContext = __primitive("outer context", localeContext);

  var a: [0..<n] int;
  __primitive("hoist to context", localeContext, a);

  var b;
  { // AST is too complicated without this block
    b = new barrier(1);
    b.multiply(1); // this no-op ensures multiply() is available to compiler
  }
  __primitive("hoist to context", localeContext, b);

  b.barrier();

  if localTaskContext.taskId == 0 then
    a[i] = i%10;

  b.barrier();

  if localTaskContext.taskId == 1 then
    writeln("%2i: ".format(i), a);
}

if doVerboseMem then stopVerboseMem();
