use HashedDist;

var D: domain(real) dmapped Hashed(idxType=real);

var inds = [2.1];

D.bulkAdd(inds);

// check serial iterator
writeln("D (serial iterator) :");
forall d in D {
  writeln(d);
}
