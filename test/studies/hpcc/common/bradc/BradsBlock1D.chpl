// TODO: Get rid of this
type indexType = int(64);

// TODO: move assumptions about Locales/LocaleSpace into a targetLocales
// field

class Block1DDist {
  const bbox: domain(1, indexType);

  def newDomain(inds) {
    return new Block1DDom(this, inds);
  }

  def procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  def getChunk(inds, localeID) {
    const lo = bbox.low;
    const hi = bbox.high;
    const numelems = hi - lo + 1;
    const blo = procToData((numelems: real * localeID) / numLocales, lo);
    const bhi = procToData((numelems: real * (localeID+1)) / numLocales, lo) - 1;
    return inds[blo..bhi];  // using domain slicing to intersect
    // In retrospect, I think the above is incorrect -- instead, compute
    // each locale's piece of R**1 once and then slice that with inds?
  }
  
  def ind2loc(ind: indexType): index(LocaleSpace) {
    return (((ind-bbox.low)*numLocales)/bbox.numIndices): index(LocaleSpace);
  }
}


class Block1DDom {
  // parameterize this by indexType and/or locIndexType
  const dist: Block1DDist;
  const whole: domain(1, indexType);
  //  SHOULD BE: const locDom: [i in LocalesDom] domain(1, indexType) = new LocBlockDom(whole);
  // TODO: Move LocaleSpace into a member variable (throughout code)
  var locDom: [LocaleSpace] LocBlock1DDom;

  def initialize() {
    [i in LocaleSpace] on Locales(i) do
      locDom(i) = new LocBlock1DDom(this, dist.getChunk(whole, here.id));
    //    [i in LocaleSpace] writeln(i, " owns ", locDom(i));
  }

  def these() {
    for blk in locDom do
      // Want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk do
          yield ind;
  }

  def writeThis(x:Writer) {
    x.write(whole);
  }

  def newArray(type elemType) {
    return new Block1DArr(elemType, this);
  }

  def numIndices {
    return whole.numIndices;
  }

  def low {
    return whole.low;
  }

  def high {
    return whole.high;
  }
}


class LocBlock1DDom {
  const wholeDom: Block1DDom;
  // parameterize this by indexType and/or locIndexType
  var myBlock: domain(1, indexType);

  def these() {
    // Want to do something like:     
    // on this do
    // But can't currently have yields in on clauses
    for ind in myBlock do
      yield ind;
  }

  def writeThis(x:Writer) {
    x.write(myBlock);
  }

  def numIndices {
    return myBlock.numIndices;
  }

  def low {
    return myBlock.low;
  }

  def high {
    return myBlock.high;
  }
}


class Block1DArr {
  type elemType;
  var dom: Block1DDom;
  var locArr: [LocaleSpace] LocBlock1DArr(elemType);

  def initialize() {
    [i in LocaleSpace] on Locales(i) do
      locArr(i) = new LocBlock1DArr(elemType, dom.locDom(i));
  }

  def this(i: indexType) var {
    return locArr(dom.dist.ind2loc(i))(i);
  }

  def these() var {
    for loc in LocaleSpace {
      // Want to do something like:     
      // on this do
      // But can't currently have yields in on clauses
      for elem in locArr(loc) {
        yield elem;
      }
    }
  }

  def writeThis(x: Writer) {
    var first = true;
    for loc in LocaleSpace {
      // Would like to do this, but it causes deadlock:
      //      on Locales(loc) {
      // See writeThisUsingOn.chpl
        if (locArr(loc).numElements >= 1) {
          if (first) {
            first = false;
          } else {
            x.write(" ");
          }
          x.write(locArr(loc));
        }
        //    }
      stdout.flush();
    }
  }

  def numElements {
    return dom.numIndices;
  }
}


class LocBlock1DArr {
  type elemType;
  const locDom: LocBlock1DDom;
  var myElems: [locDom.myBlock] elemType;

  def this(i: indexType) var {
    return myElems(i);
  }

  def these() var {
    for elem in myElems {
      yield elem;
    }
  }

  def writeThis(x: Writer) {
    // Would like to do this, but it causes deadlock:
    //    on this.locale do
    // See writeThisUsingOn.chpl
      x.write(myElems);
  }

  def numElements {
    return myElems.numElements;
  }

}
