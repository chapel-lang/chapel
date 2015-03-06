config const lo = 9;
config const hi = 17;
config const noisy = false;

const D = {lo..hi, (lo..hi)+1, (lo..hi)+2, (lo..hi)+3};
var A: [D] real;

writeln("=== In forwardDefaultArr ===");
iter myThese(A,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity) {
  for i in A._value.these() do yield i;
}

iter myThese(param tag: iterKind, A,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity)
  where tag==iterKind.leader {
  for followThis in A._value.these(iterKind.leader, dptpl, ir, mg) do
    yield followThis;
}

iter myThese(param tag: iterKind, followThis, A,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity)
  where tag==iterKind.follower {
  for i in A._value.these(iterKind.follower, followThis, dptpl, ir, mg) do
    yield i;
}

var i = 0.0;
forall a in A with (ref i) {  // race
  a = i;
  i += 0.25;
  if noisy then writeln(a);
}
forall a in myThese(A) do if noisy then writeln(a);
forall a in myThese(A, dptpl=dataParTasksPerLocale/2) do
  if noisy then writeln(a);
forall a in myThese(A, ir=!dataParIgnoreRunningTasks) do
  if noisy then writeln(a);
forall a in myThese(A, mg=dataParMinGranularity*2) do
  if noisy then writeln(a);

forall a in A._value.these() do
  if noisy then writeln(a);
forall a in A._value.these(tasksPerLocale=dataParTasksPerLocale/2) do
  if noisy then writeln(a);
forall a in A._value.these(ignoreRunning=!dataParIgnoreRunningTasks) do
  if noisy then writeln(a);
forall a in A._value.these(minIndicesPerTask=dataParMinGranularity*2) do
  if noisy then writeln(a);

writeln("=== leaving forwardDefaultArr ===");
