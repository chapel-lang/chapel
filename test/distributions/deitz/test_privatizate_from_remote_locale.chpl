use BlockDist;

config const n: int = 10;

proc main() {
  on rootLocale.getLocale(2) {
    const Dist = new dmap(new Block(rank=1,idxType=int,boundingBox={1..n},
                                    dataParTasksPerLocale=1,
                                    dataParIgnoreRunningTasks=true));
    const D: domain(1) dmapped Dist = {1..n};
    for loc in rootLocale.getLocales() do on loc do
      writeln(here.id, ": ", D);
  }
}
