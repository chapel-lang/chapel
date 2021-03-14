use BlockDist;

config const n: int = 10;

proc main() {
  on Locales(2) {
    const Dist = new dmap(new Block(rank=1,idxType=int,boundingBox={1..n},
                                    dataParTasksPerLocale=1,
                                    dataParIgnoreRunningTasks=true));
    const D: domain(1) dmapped Dist = {1..n};
    for loc in Locales do on loc do
      writeln(here.id, ": ", D);
  }
}
