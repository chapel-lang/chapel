use HashedDist;

var D: domain(real) dmapped new hashedDist(idxType=real);
D += 1.3;

var localD: domain(real);
localD += 2.1;

D = localD;

// check serial iterator
writeln("D (serial iterator) :");
forall d in D {
  writeln(d);
}
