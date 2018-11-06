config const lo = 9;
config const hi = 17;
config const noisy = false;

const D = {lo..hi, (lo..hi)+1, (lo..hi)+2, (lo..hi)+3};

writeln("=== In forwardDefaultDom ===");
iter myThese(D,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity) {
  for i in D.these() do yield i;
}

iter myThese(param tag: iterKind, D,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity)
  where tag==iterKind.leader {
  for followThis in D.these(iterKind.leader, dptpl, ir, mg) do
    yield followThis;
}

iter myThese(param tag: iterKind, followThis, D,
             dptpl=dataParTasksPerLocale,
             ir=dataParIgnoreRunningTasks,
             mg=dataParMinGranularity)
  where tag==iterKind.follower {
  for i in D.these(iterKind.follower, followThis, dptpl, ir, mg) do
    yield i;
}

forall i in D do if noisy then writeln(i);
forall i in myThese(D) do if noisy then writeln(i);
forall i in myThese(D, dptpl=dataParTasksPerLocale/2) do
  if noisy then writeln(i);
forall i in myThese(D, ir=!dataParIgnoreRunningTasks) do
  if noisy then writeln(i);
forall i in myThese(D, mg=dataParMinGranularity*2) do
  if noisy then writeln(i);

forall i in D.these() do
  if noisy then writeln(i);
forall i in D.these(tasksPerLocale=dataParTasksPerLocale/2) do
  if noisy then writeln(i);
forall i in D.these(ignoreRunning=!dataParIgnoreRunningTasks) do
  if noisy then writeln(i);
forall i in D.these(minIndicesPerTask=dataParMinGranularity*2) do
  if noisy then writeln(i);

writeln("=== leaving forwardDefaultDom ===");
