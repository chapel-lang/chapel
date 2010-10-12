enum DistType { default };

config param distType: DistType = DistType.default;

config type intType = int;
config type uintType = uint(64);
config type realType = real(32);
config type stringType = string;

def setupDistributions() {
  if distType == DistType.default {
    return (
            new dmap(new DefaultDist()),
            new dmap(new DefaultDist()),
            new dmap(new DefaultDist()),
            new dmap(new DefaultDist())
           );
  }
}

const (DistIntType, DistUintType, DistRealType, DistStringType) = setupDistributions();

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
