use SparseBlockDist;

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
