// TODO: Need to put in some on clauses throughout this code

// TODO: Get rid of this
type indexType = int(64);

def BlockPartition(D, myBlock, numBlocks) {
  const lo = D.dim(1)._low;
  const hi = D.dim(1)._high;
  const numelems = hi - lo + 1;
  const nbAsIndexType = numBlocks:index(D);
  const myBlockAsIndexType:index(D) = myBlock;
  const blo = lo + (myBlockAsIndexType*numelems) / nbAsIndexType;
  const bhi = lo + (((myBlockAsIndexType+1)*numelems) / nbAsIndexType) - 1;
  return [blo..bhi];
}

class Block1DDom {
  // parameterize this by indexType and/or locIndexType
  const whole: domain(1, indexType);
  //  SHOULD BE: const locDom: [i in LocalesDom] domain(1, indexType) = new LocBlockDom(whole);
  // TODO: Move LocalesDomain into a member variable (throughout code)
  var locDom: [LocalesDomain] LocBlock1DDom;

  def initialize() {
    [i in LocalesDomain] locDom(i) = new LocBlock1DDom(i, whole);
  }

  def these() {
    for blk in locDom do
      for ind in blk do
        yield ind;
  }

  def ind2loc(ind: indexType): index(LocalesDomain) {
    return ((ind-low)/numIndices): index(LocalesDomain);
  }


  def newArray(type elemType) {
    return new Block1DArr(elemType, this);
  }

  def dom {
    return whole;
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
  // parameterize this by indexType and/or locIndexType
  const localeID: index(LocalesDomain);
  var myBlock: domain(1, indexType);

  def LocBlock1DDom(_localeID, Whole: domain(1, indexType)) {
    localeID = _localeID;  // is there a cooler way to do this?
    myBlock = BlockPartition(Whole, localeID, numLocales);
  }

  def these() {
    for ind in myBlock do
      yield ind;
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
  var locArr: [LocalesDomain] LocBlock1DArr(elemType);

  def initialize() {
    [i in LocalesDomain] locArr(i) = new LocBlock1DArr(elemType, dom.locDom(i));
  }

  def this(i: indexType) var {
    return locArr(dom.ind2loc(i))(i);
  }

  def these() var {
    for loc in LocalesDomain {
      for elem in locArr(loc) {
        yield elem;
      }
    }
  }

  def writeThis(x: Writer) {
    var first = true;
    for loc in LocalesDomain {
      if (locArr(loc).numElements >= 1) {
        if (first) {
          first = false;
        } else {
          x.write(" ");
        }
        x.write(locArr(loc));
      }
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
    x.write(myElems);
  }

  def numElements {
    return myElems.numElements;
  }

}
