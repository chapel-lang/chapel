class BlockCyclicDist {
  type idxType;
  param nDims:int = 2;
  const blockSize: nDims*int;
  const startLoc:  nDims*int;
  const localeDomain: domain(nDims);
  const locales: [localeDomain] locale;

  proc idxToLocaleInd(ind: idxType...nDims) {
    var locInd: nDims*idxType;
    for i in 1..nDims {
      locInd(i) = (startLoc(i) + (ind(i)-1)/blockSize(i)) % localeDomain.dim(i).size;
    }
    return locInd;
  }

  proc idxToLocale(ind: idxType...nDims) {
    return locales(idxToLocaleInd((...ind)));
  }
  proc getBlock(ind: idxType...nDims) {
    var locInd: nDims*idxType;
    for i in 1..nDims {
      locInd(i) = (ind(i) - 1) / (localeDomain.dim(i).size*blockSize(i));
    }
    return locInd;
  }
  proc getBlockPosition(ind: idxType...nDims) {
    var locInd: nDims*idxType;
    for i in 1..nDims {
      locInd(i) = ((ind(i)-1) % blockSize(i)) + 1;
    }
    return locInd;
  }
  proc getLocalPosition(ind:idxType...nDims) {
    var blk = getBlock((...ind));
    var blkPos = getBlockPosition((...ind));
    var position: nDims*idxType;
    for i in 1..nDims {
      position(i) = blk(i)*blockSize(i) + blkPos(i);
    }
    return position;
  }

  proc buildDomain(d: domain(nDims)) {
    return new unmanaged BlockCyclicDom(nDims, idxType, d, _to_unmanaged(this));
  }

}

class BlockCyclicDom {
  param nDims: int = 2;
  type idxType;
  const whole: domain(nDims, idxType);
  const dist: unmanaged BlockCyclicDist(idxType);
  var locDoms: [dist.localeDomain] unmanaged LocBlockCyclicDom(nDims, idxType)?;
  proc postinit() {
    var blksInDim: nDims*idxType;
    for i in 1..nDims {
      blksInDim(i) = ceil(whole.dim(i).size:real(64) /
                          dist.blockSize(i)):idxType;
    }

    for pos in dist.localeDomain {
      var locSize: nDims*idxType;
      
      for dim in 1..nDims {
        var remainder: idxType;
        locSize(dim) = dist.blockSize(dim) * (blksInDim(dim) / dist.localeDomain.dim(dim).size);
        remainder = whole.dim(dim).size - (locSize(dim) * dist.localeDomain.dim(dim).size);
        if ((1+pos(dim))*dist.blockSize(dim) <= remainder) {
          locSize(dim) += dist.blockSize(dim);
        } else if (remainder - pos(dim)*dist.blockSize(dim) > 0) {
          locSize(dim) += remainder - pos(dim)*dist.blockSize(dim);
        }
      }
      on dist.locales(pos) {
        var locRanges: nDims*range(idxType, BoundedRangeType.bounded);
        for i in 1..nDims {
          locRanges(i) = 1..locSize(i);
        }
        locDoms(pos) = new unmanaged LocBlockCyclicDom(nDims, idxType, _to_unmanaged(this), locRanges);
      }
      //writeln(pos, locSize);
    }
  }

  proc deinit() {
    for loc in dist.localeDomain do
      on dist.locales(loc) do
        delete locDoms(loc);
  }

  proc buildArray(type eltType) {
    return new unmanaged BlockCyclicArr(nDims, idxType, eltType, _to_unmanaged(this));
  }

}


class LocBlockCyclicDom {
  param nDims: int;
  type idxType;
  const whole: unmanaged BlockCyclicDom(nDims, idxType);
  const locRanges: nDims*range(idxType, BoundedRangeType.bounded);
  const locDom: domain(nDims);
  proc postinit() {
    locDom.setIndices(locRanges);
  }
}


class BlockCyclicArr {
  param nDims: int;
  type idxType;
  type eltType;
  const dom: unmanaged BlockCyclicDom(nDims, idxType);
  var locArrs: [dom.dist.localeDomain] unmanaged LocBlockCyclicArr(nDims, idxType, eltType)?;
  proc postinit() {
    for ind in dom.dist.localeDomain {
      on dom.dist.locales(ind) {
        locArrs(ind) = new unmanaged LocBlockCyclicArr(nDims, idxType, eltType, _to_unmanaged(this), dom.locDoms(ind)!);
      }
    }
  }
  proc deinit() {
    for ind in dom.dist.localeDomain do
      on dom.dist.locales(ind) do
        delete locArrs(ind);
  }

  proc this(ind:idxType...nDims) ref {
    return locArrs(dom.dist.idxToLocaleInd((...ind)))!.arr(dom.dist.getLocalPosition((...ind)));
  }
}

class LocBlockCyclicArr {
  param nDims: int;
  type idxType;
  type eltType;
  const whole: unmanaged BlockCyclicArr(nDims, idxType, eltType);
  const locDom: unmanaged LocBlockCyclicDom(nDims, idxType);
  const arr: [locDom.locDom] eltType;
}

config const m, n = 2;


proc main {
  var locDom: domain(2) = {0..#m, 0..#n};
  var locs: [locDom] locale =
        [(i,j) in locDom] Locales((i*n + j) % numLocales);
  var undistributedDom: domain(2) = {1..5, 1..5};

  var dist = new unmanaged BlockCyclicDist(idxType=int, nDims=2,
                                   blockSize=(2,2), startLoc=(0,0),
                                   localeDomain=locDom, locales=locs);
  var dom = dist.buildDomain(undistributedDom);
  var arr = dom.buildArray(int);
  for (i,j) in undistributedDom {
    arr(i,j) = i*10 + j;
  }
  for (i,j) in undistributedDom {
    writeln((i,j), " ", arr(i,j));
    //writeln((i,j), " ", arr(i,j), " ", arr(i,j).locale);
  }
  delete arr;
  delete dom;
  delete dist;
}
