use BlockDist;

config param distString: string = 'default';

config const n1 = 100;
config const n2 = 20;
config const n3 = 5;
config const n4 = 3;
config const n5 = max(int(64));

const Space1 = [1..n1];
const Space2 = [1..n2, 1..n2];
const Space3 = [1..n3, 1..n3, 1..n3];
const Space4 = [1..n4, 1..n4, 1..n4, 1..n4];
const Space2D64 = [n5-8..n5, n5-8..n5];

def setupDistributions() {
  if distString == 'default' {
    return (
            distributionValue(new DefaultDist()),
            distributionValue(new DefaultDist()),
            distributionValue(new DefaultDist()),
            distributionValue(new DefaultDist()),
            distributionValue(new DefaultDist())
           );
  }
  if distString == 'block' {
    return (
            distributionValue(new Block(rank=1, bbox=Space1)),
            distributionValue(new Block(rank=2, bbox=Space2)),
            distributionValue(new Block(rank=3, bbox=Space3)),
            distributionValue(new Block(rank=4, bbox=Space4)),
            distributionValue(new Block(rank=2, idxType=int(64), bbox=Space2D64))
           );
  }
}

const (Dist1D, Dist2D, Dist3D, Dist4D, Dist2D64) = setupDistributions();
