class DimensionDistributor {
  // interface
  proc indToDimInd(ind: int, localeDomain) { return 0; }
  proc localPart(localeDomain, loc, type idxType) { return 1..0 by 1; }
}

class Cyclic: DimensionDistributor {
  param myDimension:int;
  proc mod(a, b) {
    var tmp = a % b;
    return if tmp < 0 then b + tmp else tmp;
  }

  override proc indToDimInd(ind: int, localeDomain) {
    return mod(ind, localeDomain.dim(myDimension).size);
  }

  override proc localPart(localeDomain, loc, type idxType) {
    var lows:[localeDomain.dim(myDimension)] idxType;
    var length = localeDomain.dim(myDimension).size;
    for i in min(idxType)..#length {
      lows(mod(i, length)) = i;
    }
    return lows(loc(myDimension))..max(idxType) by length;
  }
}

class Dimensional {
  param nDims: int;
  type idxType = int;
  var dimensionDistributors: nDims*unmanaged DimensionDistributor;
  var localeDomain: domain(nDims);
  var localeArray: [localeDomain] locale;


  proc indexToLoc(ind:idxType ...nDims) {
    return localeArray(indexToLocIndex((...ind)));
  }

  // Return the index into the localeArray that the ind maps to
  // using the dimensionDistributer for each dimension
  proc indexToLocIndex(ind: idxType ...nDims) {
    var localeArrayInd: nDims*int;
    for i in 1..#nDims {
       localeArrayInd(i) = dimensionDistributors(i).indToDimInd(ind(i), localeDomain);
    }
    return localeArrayInd;
  }

  proc indexToLocIndex(ind: nDims*idxType) {
    return indexToLocIndex((...ind));
  }

  proc newDomain(inds:domain(nDims,idxType)) {
    return new unmanaged DimensionalDomain(nDims, idxType, inds, _to_unmanaged(this));
  }

  // map an index into the localeArray to a tuple of ranges with
  // each position in the tuple representing all values that would be
  // mapped to that locale for that dimension within the range
  // min(idxType)..max(idxType)
  proc getLocRanges(loc: nDims*int) {
    var ranges: nDims*range(stridable=true);
    for param dim in 1..nDims {
      ranges(dim) = dimensionDistributors(dim).localPart(localeDomain, loc, idxType);
    }
    return ranges;
  }
}

enum IteratorType { leader, follower };

class DimensionalDomain {
  param nDims: int;
  type idxType;
  var whole: domain(nDims, idxType);
  var dist:unmanaged Dimensional(nDims, idxType);
  var locDoms: [dist.localeDomain] unmanaged LocDimensionalDomain(nDims, idxType)?;

  proc postinit() {
    for loc in dist.localeDomain {
      on dist.localeArray(loc) {
        var locDist = dist.getLocRanges(loc);
        locDoms(loc) = new unmanaged LocDimensionalDomain(nDims, idxType, _to_unmanaged(this), whole((...locDist)));
      }
    }
  }

  proc deinit() {
    for loc in dist.localeDomain {
      on dist.localeArray(loc) {
        delete locDoms(loc);
      }
    }
  }

  iter these() {
    for i in whole {
      yield i;
    }
  }

  iter newThese(param iteratorType:IteratorType)
    where iteratorType == IteratorType.leader {
    for locDom in locDoms {
      yield locDom!.myElems; // - whole.low;
    }
  }
  iter newThese(param iteratorType:IteratorType, followThis)
    where iteratorType == IteratorType.follower {
    for i in followThis {
      yield i; // + whole.low;
    }
  }


  proc newArray(type eltType) {
    return new unmanaged DimensionalArray(nDims, idxType, eltType, _to_unmanaged(this));
  }
}

class LocDimensionalDomain {
  param nDims: int;
  type idxType;
  var whole: unmanaged DimensionalDomain(nDims, idxType);
  var myElems: domain(nDims, idxType, true);
}

class DimensionalArray {
  param nDims: int;
  type idxType;
  type eltType;
  var dom: unmanaged DimensionalDomain(nDims, idxType);
  var locArrs: [dom.dist.localeDomain] unmanaged LocDimensionalArray(nDims, idxType, eltType)?;

  proc postinit() {
    for loc in dom.dist.localeDomain {
      on loc {
        locArrs(loc) = new unmanaged LocDimensionalArray(nDims, idxType, eltType, _to_unmanaged(this), dom.locDoms(loc)!);
      }
    }
  }

  proc deinit() {
    for loc in dom.dist.localeDomain {
      on loc {
        delete locArrs(loc);
      }
    }
  }

  proc this(ind: nDims*idxType) ref {
    return locArrs(dom.dist.indexToLocIndex(ind))!.locArr(ind);
  }

  iter newThese(param iteratorType:IteratorType)
    where iteratorType == IteratorType.leader {
    for blk in dom.newThese(IteratorType.leader) do
      yield blk;
  }

  iter newThese(param iteratorType:IteratorType, followThis) ref
    where iteratorType == IteratorType.follower {
    for i in followThis {
      yield this(i);
    }
  }
}

class LocDimensionalArray {
  param nDims: int;
  type idxType;
  type eltType;
  var whole: unmanaged DimensionalArray(nDims, idxType, eltType);
  var locBlk: unmanaged LocDimensionalDomain(nDims, idxType);
  var locArr: [locBlk.myElems] eltType;
}


proc main {
  param nDims = 2;
  param nLocRows = 2;
  param nLocCols = 3;
  var localeDom: domain(nDims) = {0..#nLocRows, 0..#nLocCols};
  var locales: [localeDom] locale =
    for (i,j) in localeDom do Locales((i*nLocCols + j)%numLocales);

  var dims: nDims*unmanaged DimensionDistributor = (new unmanaged Cyclic(1), new unmanaged Cyclic(2));
  var dist = new unmanaged Dimensional(2, int, dims, localeDom, locales);
  var dom = dist.newDomain({0..3,0..5});

  var arr = dom.newArray(real);
  var arr2= dom.newArray(real);
  var r: real = 0.0;
  for blk in arr.newThese(IteratorType.leader) {
    on blk {
      writeln(here.id, ": blk is: ", blk);
      for (elt,elt2) in zip( arr.newThese(IteratorType.follower, blk),
                         arr2.newThese(IteratorType.follower, blk)) {
         elt = r;
         elt2 = r*2;
         r = r + 1.0;
      }
    }
  }

  for i in dom {
    writeln(i, (arr(i), arr2(i)));
  }
  delete arr2;
  delete arr;
  delete dom;
  delete dist;
  delete dims(1);
  delete dims(2);
}
