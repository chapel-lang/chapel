use HashedDist;

record MyMapper {
  proc this(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.domain.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

proc doit() {
  var newDist = new dmap(new unmanaged Hashed(idxType=real, mapper=new MyMapper()));

  var D: domain(real) dmapped newDist;

  var loc0index = 0.1;
  var loc1index = 1.3;

  D += loc0index;
  D += loc1index;

  var A:[D] int;

  var value1 = 10;
  var value2 = 20;

  A = value1;

  if numLocales > 1 then on Locales[1] {
    const idxTemp = loc1index;
    local {
      // Access the array.
      // This statement succeeds if there is no communication.
      // Check that idxTemp is in the domain
      assert(D.contains(idxTemp));
      assert(A.domain.contains(idxTemp));
    }
  }
  
  // This case is less interesting
  // since if distribution/privitization failed, extra
  // stuff will be on locale 0...
  on Locales[0] {
    const idxTemp = loc0index;
    local {
      // Access the array.
      // This statement succeeds if there is no communication.
      assert(D.contains(idxTemp));
      assert(A.domain.contains(idxTemp));
    }
  }

}

doit();
