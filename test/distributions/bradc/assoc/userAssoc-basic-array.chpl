use UserMapAssoc;

class MyMapper {
  proc indexToLocaleIndex(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.domain.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

var newDist = new dmap(new unmanaged UserMapAssoc(idxType=real, mapper=new unmanaged MyMapper()));

var D: domain(real) dmapped newDist;

D += 1.3;

var A: [D] real;

A[1.3] = 2.6;

// check serial iterator
writeln("A (serial iterator) :");
for a in A {
  writeln(a);
}

writeln("A (leader/follower iterator) :");
// check parallel leader-follower iterator
forall (a1,a2) in zip(A,A) {
  writeln(a1, " ", a2);
}

writeln("A (standalone iterator) :");
forall a in A {
  writeln(a);
}


