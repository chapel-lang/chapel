use BlockDist;

config param distString: string = 'block';

def setupDistributions() {
  if distString == 'block' {
    return (
            distributionValue(new Block(rank=1, bbox=[1..5])),
            distributionValue(new Block(rank=2, bbox=[1..5,1..5])),
            distributionValue(new Block(rank=3, bbox=[1..5,1..5,1..5])),
            distributionValue(new Block(rank=4, bbox=[1..5,1..5,1..5,1..5]))
           );
  }
}

const (Dist1D, Dist2D, Dist3D, Dist4D) = setupDistributions();
