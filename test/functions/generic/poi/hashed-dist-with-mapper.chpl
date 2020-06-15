/*
This code is a simplification of:
  arrays/userAPI/assocDistributedHashed.chpl

specifically:
  use HashedDist;
  record MyMapper {
    proc this(ind, targetLocs: [] locale) {
      return targetLocs.domain.low;
    }
  }
  proc main() {
    var myMapper = new MyMapper();
    var D: domain(string) dmapped Hashed(idxType=string, mapper=myMapper);
    D += "zero";
  }
*/

module Application {
  use MyChapelArray;
  use MyHashedDist;

  record MyMapper {
    proc this(ind) {  // to be invoked from indexToLocaleIndex()
      compilerError("success");
    }
  }

  proc main() {
    var myMapper = new MyMapper();
    var D = new HashedDomain(myMapper);
    add(D, "zero");
  }
}

module MyChapelArray {
  use MyHashedDist;
  proc add(D, elm) {
    D._value.dsiAdd(elm);
  }
}

module MyHashedDist {
  class HashedDomain {
    var mapper;
    proc _value return this;
    proc dsiAdd(elm) {
      indexToLocaleIndex(elm);
    }
    proc indexToLocaleIndex(elm) {
      mapper(elm);  // need visible fns from caller scope / POI
    }
  }
}
