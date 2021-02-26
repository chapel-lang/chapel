use HashedDist;

class MyMapper {
  proc this(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.domain.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

var myMapper = lambda(ind:real, targetLocs: Locales.type) {
                        const numlocs = targetLocs.domain.size;
                        const indAsInt = ind: int;
                        return indAsInt % numlocs;
                     };

var D: domain(real) dmapped Hashed(idxType=real, mapper=new MyMapper());

D += 1.3;
D += 22.0;
D += 16;
D += 99;
D += 0.3;
D += 7185;

forall d in D {
  assert(here == Locales[myMapper(d, Locales)]);
}
