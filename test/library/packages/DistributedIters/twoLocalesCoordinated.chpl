use DistributedIters;

forall i in distributedDynamic(1..10, numTasks=1, chunkSize=2, coordinated=true) do
  writeln(here, ": doing iteration ", i);

if (numLocales > 1) {
  on Locales[numLocales-1] {
    writeln("On locale 1 now");
    forall i in distributedDynamic(1..10, numTasks=1, chunkSize=2, coordinated=true) do
      writeln(here, ": doing iteration ", i);
  }
}
