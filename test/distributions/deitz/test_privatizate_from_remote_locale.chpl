use BlockDist;

config const n: int = 10;

def main() {
  on Locales(2) {
    const Dist = new dist(new Block(rank=1,idxType=int,boundingBox=[1..n],
                                    maxDataParallelism=1,
                                    limitDataParallelism=false));
    const D: domain(1) distributed Dist = [1..n];
    for loc in Locales do on loc do
      writeln(here.id, ": ", D);
  }
}
