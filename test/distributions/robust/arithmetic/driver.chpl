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
            new dist(new DefaultDist()),
            new dist(new DefaultDist()),
            new dist(new DefaultDist()),
            new dist(new DefaultDist()),
            new dist(new DefaultDist())
           );
  }
  if distString == 'block' {
    return (
            new dist(new Block(rank=1, bbox=Space1)),
            new dist(new Block(rank=2, bbox=Space2)),
            new dist(new Block(rank=3, bbox=Space3)),
            new dist(new Block(rank=4, bbox=Space4)),
            new dist(new Block(rank=2, idxType=int(64), bbox=Space2D64))
           );
  }
}

const (Dist1D, Dist2D, Dist3D, Dist4D, Dist2D64) = setupDistributions();

const Dom1D: domain(1) distributed Dist1D = Space1;
const Dom2D: domain(2) distributed Dist2D = Space2;
const Dom3D: domain(3) distributed Dist3D = Space3;
const Dom4D: domain(4) distributed Dist4D = Space4;
const Dom2D64: domain(2,int(64)) distributed Dist2D64 = Space2D64;

var A1D: [Dom1D] int;
var A2D: [Dom2D] int;
var A3D: [Dom3D] int;
var A4D: [Dom4D] int;
var A2D64: [Dom2D64] int;

def outputArrays() {
  write("A1D  :"); for i in Space1 do write(" ", A1D[i]); writeln();
  write("A2D  :"); for i in Space2 do write(" ", A2D[i]); writeln();
  write("A3D  :"); for i in Space3 do write(" ", A3D[i]); writeln();
  write("A4D  :"); for i in Space4 do write(" ", A4D[i]); writeln();
  write("A2D64:"); for i in Space2D64 do write(" ", A2D64[i]); writeln();
}

//
// creates a tuple of size 'rank' initialized with values 'x'
//
def fill(param rank, x) {
  if rank == 1 {
    var y: 1*x.type;
    y(1) = x;
    return y;
  } else if rank == 2 {
    return (x, x);
  } else {
    return (x, (...fill(rank-1, x)));
  }
}

//
// creates a domain of rank 'rank' with each dimension equal to 'extent'
//
def rankDomain(param rank, extent) {
  return [(...fill(rank, extent))];
}

//
// returns a next integer starting at 1
//
var next_i = 0;
def next() { next_i += 1; return next_i; }
