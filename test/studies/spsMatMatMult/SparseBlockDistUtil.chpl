// This module defines routines that should arguably be defined in
// SparseBlockDist.chpl, but aren't currently.
//
module SparseBlockDistUtil {
  use SparseBlockDist;

  proc SparseBlockDom.dsiTargetLocales() const ref {
    return dist.targetLocales;
  }

  proc SparseBlockDom.setLocalBlock(locDom) {
    this.myLocDom!.mySparseBlock = locDom;
  }

  proc SparseBlockArr.dsiTargetLocales() const ref {
    return dom.dsiTargetLocales();
  }

  proc SparseBlockArr.getBlock(localeRow, localeCol) const ref {
    return this.locArr[localeRow, localeCol]!.myElems;
  }
  
  proc SparseBlockArr.setLocalBlock(locArr) {
    this.myLocArr!.myElems._value.data = locArr.data;
  }
}

/*
proc LocSparseBlockDom.localize() {
  return new unmanaged LocSparseBlockDom(this.rank, this.idxType, this.strides,
                                         this.sparseLayoutType, this.parentDom,
                                         this.sparseDist, this.mySparseBlock);
}

// TODO: Need to free this
proc LocSparseBlockArr.localize(locDom) {
  const locClass = new unmanaged LocSparseBlockArr(this.eltType, this.rank,
                                                   this.idxType, this.strides,
                                                   this.sparseLayoutType,
                                                   locDom,
                                                   initElts=false);
  locClass.myElems = this.myElems;
  return locClass;
}
*/