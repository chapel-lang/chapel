use BlockDist;

config const n: int = 10;

def main() {
  on Locales(2) {
    const Dist = distributionValue(new Block(rank=1,idxType=int,bbox=[1..n],tasksPerLocale=1));
    const D: domain(1) distributed Dist = [1..n];
    for loc in Locales do on loc do
      writeln(here.id, ": ", D);
  }
}
