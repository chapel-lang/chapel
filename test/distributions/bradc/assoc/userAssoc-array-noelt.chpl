use HashedDist;

record MyMapper {
  proc this(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.domain.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

var newDist = new dmap(new unmanaged Hashed(idxType=real, mapper=new MyMapper()));

var D: domain(real) dmapped newDist;

D += 1.3;

var A: [D] real;

A[1.3] = 2.6;

var x = A[22];
writeln(x);

