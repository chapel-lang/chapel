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

// check serial iterator
writeln("D (serial iterator) :");
for d in D {
  writeln(d);
}

writeln("D (leader/follower iterator) :");
// check parallel leader-follower iterator
forall (d1,d2) in zip(D,D) {
  writeln(d1, " ", d2);
}

writeln("D (standalone iterator) :");
forall d in D {
  writeln(d);
}


D += 2.4;
D += 3.5;

// check .sorted()
writeln("D is:");
for d in D.sorted() {
  writeln(d);
}


