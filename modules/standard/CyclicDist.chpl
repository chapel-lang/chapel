//
// naive routine for dividing numLocales into rank factors
//
def _factor(param rank: int, value) {
  var factors: rank*int;
  for param i in 1..rank do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 1;
          for i in 2..rank {
            if factors(i) < factors(j) then
              j = i;
          }
          factors(j) *= i;
          iv = iv / i;
          break;
        }
      }
    }
  }
  for i in 1..rank do
    for j in i+1..rank do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}

def _tupleOfZero(param size: int, type t) {
  var tmp: size * t;
  return tmp;
}

config param debugCyclicDist = false;
config param verboseCyclicDistWriters = false;


class Cyclic: BaseDist {
  param rank: int = 1;
  type idxType = int(64);

  const lowIdx: rank*idxType;
  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;

  const locDist: [targetLocDom] LocCyclic(rank, idxType);

  const tasksPerLocale = 1;
  var pid: int = -1;

  def Cyclic(param rank: int,
             type idxType = int(64),
             low: rank*idxType = _tupleOfZero(rank, idxType),
             targetLocales: [] locale = thisRealm.Locales,
             tasksPerLocale=0) {
    if rank == 1  {
      targetLocDom = [0..#targetLocales.numElements];
      targetLocs = targetLocales;
    } else if targetLocales.rank == 1 {
      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank {
        ranges(i) = 0..#factors(i);
      }
      targetLocDom = [(...ranges)];
      for (loc1, loc2) in (targetLocs, targetLocales) {
        loc1 = loc2;
      }
    } else {
      if targetLocales.rank != rank then
        compilerError("locales array rank must be one or match distribution rank");
      var ranges: rank*range;
      for param i in 1..rank do {
        var thisRange = targetLocales.domain.dim(i);
        ranges(i) = 0..#thisRange.length; 
      }
      targetLocDom = [(...ranges)];
      targetLocs = reshape(targetLocales, targetLocDom);
    }

    for param i in 1..rank do
      lowIdx(i) = mod(low(i), targetLocDom.dim(i).length);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = min reduce targetLocales.numCores;
    else
      this.tasksPerLocale = tasksPerLocale;
    coforall locid in targetLocDom {
      on targetLocs(locid) {
        locDist(locid) = new LocCyclic(rank, idxType, locid, this);
      }
    }
    if debugCyclicDist then
      for loc in locDist do writeln(loc);
  }

  def Cyclic(param rank, type idxType, other: Cyclic(rank, idxType)) {
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    lowIdx = other.lowIdx;
    locDist = other.locDist;
    tasksPerLocale = other.tasksPerLocale;
  }

  def clone() return new Cyclic(rank=rank, idxType=idxType, other=this);
}


def Cyclic.getChunk(inds, locid) {
  var sliceBy: rank*range(eltType=idxType, stridable=true);
  var locidtup: rank*int;
  if rank == 1 then
    locidtup(1) = locid;
  else
    locidtup = locid;
  for param i in 1..rank {
    var distStride = targetLocDom.dim(i).length;
    var loclowidx = mod(lowIdx(i) + locidtup(i), distStride);
    var lowmod = mod(inds.dim(i).low, distStride);
    var offset = loclowidx - lowmod;
    if offset < 0 then
      sliceBy(i) = inds.dim(i).low + (distStride + offset)..inds.dim(i).high by distStride;
    else
      sliceBy(i) = inds.dim(i).low + offset..inds.dim(i).high by distStride;
  }
  return inds((...sliceBy));
  //
  // WANT:
  //var distWhole = locDist(locid).myChunk.getIndices();
  //return inds((...distWhole));
}

def Cyclic.supportsPrivatization() param return true;

def Cyclic.getPrivatizeData() return 0;

def Cyclic.privatize(privatizeData) {
  return new Cyclic(rank=rank, idxType=idxType, other=this);
}

def Cyclic.reprivatize(other) {
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  lowIdx = other.lowIdx;
  tasksPerLocale = other.tasksPerLocale;
}

def Cyclic.newArithmeticDom(param rank: int, type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  var dom = new CyclicDom(rank=rank, idxType=idxType, dist = this, stridable=stridable);
  dom.setup();
  return dom;
} 

def Cyclic.writeThis(x: Writer) {
  x.writeln(typeToString(this.type));
  x.writeln("------");
  for locid in targetLocDom do
    x.writeln(" [", locid, "=", targetLocs(locid), "] owns chunk: ", locDist(locid).myChunk); 
}

def Cyclic.ind2locInd(i: idxType) {
  const numLocs:idxType = targetLocDom.numIndices:idxType;
  // this is wrong if i is less than lowIdx
  //return ((i - lowIdx(1)) % numLocs):int;
  // this works even if i is less than lowIdx
  return mod(mod(i, numLocs) - mod(lowIdx(1), numLocs), numLocs):int;
}

def Cyclic.ind2locInd(ind: rank*idxType) {
  var x: rank*int;
  for param i in 1..rank {
    var dimLen = targetLocDom.dim(i).length;
    //x(i) = ((ind(i) - lowIdx(i)) % dimLen):int;
    x(i) = mod(mod(ind(i), dimLen) - mod(lowIdx(i), dimLen), dimLen):int;
  }
  return x;
}

def Cyclic.ind2loc(i: idxType) where rank == 1 {
  return targetLocs(ind2locInd(i));
}

def Cyclic.ind2loc(i: rank*idxType) {
  return targetLocs(ind2locInd(i));
}


class LocCyclic {
  param rank: int;
  type idxType;

  const myChunk: domain(rank, idxType, true);

  def LocCyclic(param rank, type idxType, locid, dist: Cyclic(rank, idxType)) {
    var locidx: rank*int;
    var lowIdx = dist.lowIdx;

    if rank == 1 then
      locidx(1) = locid;
    else
      locidx = locid;

    var tuple: rank*range(idxType, stridable=true);

    for param i in 1..rank {
      const lower = min(idxType)..(lowIdx(i)+locidx(i)) by -dist.targetLocDom.dim(i).length;
      const upper = (lowIdx(i) + locidx(i))..max(idxType) by dist.targetLocDom.dim(i).length;
      const lo = lower.low, hi = upper.high;
      tuple(i) = lo..hi by dist.targetLocDom.dim(i).length;
    }
    myChunk = [(...tuple)];
  }
}


class CyclicDom : BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  const dist: Cyclic(rank, idxType);

  var locDoms: [dist.targetLocDom] LocCyclicDom(rank, idxType, stridable);

  const whole: domain(rank, idxType, stridable);

  var pid: int = -1;
}


def CyclicDom.setup() {
  coforall localeIdx in dist.targetLocDom {
    on dist.targetLocs(localeIdx) {
      locDoms(localeIdx) = new LocCyclicDom(rank, idxType, stridable, this,
                                            dist.getChunk(whole, localeIdx));
    }
  }
}

def CyclicDom.buildArray(type eltType) {
  var arr = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

def CyclicDom.getIndices() {
  return whole.getIndices();
}

def CyclicDom.setIndices(x) {
  whole.setIndices(x);
  setup();
}

def CyclicDom.writeThis(x: Writer) {
  if verboseCyclicDistWriters {
    writeln(typeToString(this.type));
    writeln("------");
    for loc in dist.targetLocDom {
      x.writeln("[", loc, "=", dist.targetLocs(loc), "] owns ", locDoms(loc).myBlock);
    }
  } else {
    x.write(whole);
  }
}

def CyclicDom.numIndices return whole.numIndices;

def CyclicDom.these() {
  for i in whole do
    yield i;
}

def CyclicDom.these(param tag: iterator) where tag == iterator.leader {
  const wholeLow = whole.low,
        precomputedNumTasks = dist.tasksPerLocale;
  coforall locDom in locDoms do on locDom {
    const numTasks = precomputedNumTasks;

    var result: rank*range(eltType=idxType, stridable=true);
    var zeroedLocalPart = whole((...locDom.myBlock.getIndices())) - whole.low;
    for param i in 1..rank {
      var dim = zeroedLocalPart.dim(i);
      var wholestride = whole.dim(i).stride;
      if dim.high >= dim.low then
        result(i) = (dim.low / wholestride)..(dim.high / wholestride) by (dim.stride / wholestride);
      else
        result(i) = 1..0 by 1;
    }
    if numTasks == 1 {
      if debugCyclicDist then
        writeln(here.id, ": leader whole: ", whole,
                         " result: ", result,
                         " myblock: ", locDom.myBlock);
      yield result;
    } else {
      var splitRanges: rank*range(eltType=idxType, stridable=true) = result;

      coforall taskid in 0..#numTasks {
        const low = result(1).low, high = result(1).high;
        const (lo,hi) = _computeBlock(low, high - low + 1,
                                      low, high,
                                      numTasks, taskid);
        splitRanges(1) = result(1)(lo..hi);
        if debugCyclicDist then
          writeln(here.id, ": leader whole: ", whole,
                           " result: ", result,
                           " splitRanges: ", splitRanges);
        yield splitRanges;
      }
    }
  }
}

def CyclicDom.these(param tag: iterator, follower, param aligned: bool = false) where tag == iterator.follower {
  var t: rank*range(idxType, stridable=true);
  if debugCyclicDist then
    writeln(here.id, ": follower whole is: ", whole,
                     " follower is: ", follower);
  for param i in 1..rank {
    const wholestride = whole.dim(i).stride;
    t(i) = ((follower(i).low*wholestride)..(follower(i).high*wholestride) by (follower(i).stride*wholestride)) + whole.dim(i).low;
  }
  if debugCyclicDist then
    writeln(here.id, ": follower maps to: ", t);
  for i in [(...t)] do
    yield i;
}

def CyclicDom.supportsPrivatization() param return true;

def CyclicDom.getPrivatizeData() return 0;

def CyclicDom.privatize(privatizeData) {
  var distpid = dist.pid;
  var thisdist = dist;
  var privdist = __primitive("chpl_getPrivatizedClass", thisdist, distpid);
  var c = new CyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

def CyclicDom.reprivatize(other) {
  locDoms = other.locDoms;
  whole = other.whole;
}

def CyclicDom.dim(d: int) return whole.dim(d);

def CyclicDom.strideBy(str: int) {
  var alias = new CyclicDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
  var t: rank*range(eltType=idxType, stridable=true);
  for param i in 1..rank {
    t(i) = this.dim(i) by str;
  }
  alias.setIndices(t);
  return alias;
}

def CyclicDom.strideBy(str: rank*int) {
  var alias = new CyclicDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
  var t: rank*range(eltType=idxType, stridable=true);
  for i in 1..rank {
    t(i) = this.dim(i) by str(i);
  }
  alias.setIndices(t);
  return alias;
}

def CyclicDom.slice(param stridable: bool, ranges) {
  var alias = new CyclicDom(rank=rank, idxType=idxType, dist=dist, stridable=stridable||this.stridable);
  alias.setIndices(whole((...ranges)).getIndices());
  return alias;
}

def CyclicDom.localSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}


class LocCyclicDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  const wholeDom: CyclicDom(rank, idxType, stridable);
  var myBlock: domain(rank, idxType, true);
}


def _computeBlock(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  def procToData(x, lo)
    return lo + (x:lo.type) + (x:real != x:int:real):lo.type;
 
  const blo =
    if blocknum == 0 then waylo
      else procToData((numelems:real * blocknum) / numblocks, lo);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
      else procToData((numelems:real * (blocknum+1)) / numblocks, lo) - 1;

  return (blo, bhi);
}


class CyclicArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var dom: CyclicDom(rank, idxType, stridable);

  var locArr: [dom.dist.targetLocDom] LocCyclicArr(eltType, rank, idxType, stridable);
  var myLocArr: LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable);
  var pid: int = -1;
}

def CyclicArr.checkSlice(ranges) {
  for param i in 1..rank {
    if !dom.dim(i).boundsCheck(ranges(i)) {
      writeln(dom.dim(i), " ", ranges(i), " ", dom.dim(i).boundsCheck(ranges(i)));
      halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
    }
  }
}

def CyclicArr.slice(d: CyclicDom) {
  var alias = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      alias.locArr[i] = new LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i]);
      alias.locArr[i].myElems => locArr[i].myElems[alias.locArr[i].locDom.myBlock];
      alias.myLocArr = alias.locArr[i];
    }
  }
  return alias;
}

def CyclicArr.localSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }
  if rank == 1 {
    var A => locArr(dom.dist.ind2locInd(low)(1)).myElems((...ranges));
    return A;
  } else {
    var A => locArr(dom.dist.ind2locInd(low)).myElems((...ranges));
    return A;
  }
}

def CyclicArr.getBaseDom() return dom;

def CyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      locArr(localeIdx) = new LocCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

def CyclicArr.supportsPrivatization() param return true;

def CyclicArr.getPrivatizeData() return 0;

def CyclicArr.privatize(privatizeData) {
  var dompid = dom.pid;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  var c = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  c.locArr = locArr;
  for localeIdx in dom.dist.targetLocDom do
    if c.locArr(localeIdx).locale == here then
      c.myLocArr = c.locArr(localeIdx);
  return c;
}

def CyclicArr.this(i: idxType) var where rank == 1 {
  local {
    if myLocArr.locDom.myBlock.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def CyclicArr.this(i:rank*idxType) var {
  return locArr(dom.dist.ind2locInd(i))(i);
}

def CyclicArr.these() var {
  for i in dom do
    yield this(i);
}

def CyclicArr.these(param tag: iterator) where tag == iterator.leader {
  const wholeLow = dom.whole.low,
        precomputedNumTasks = dom.dist.tasksPerLocale;
  coforall locDom in dom.locDoms do on locDom {
    const numTasks = precomputedNumTasks;

    var result: rank*range(eltType=idxType, stridable=true);
    var zeroedLocalPart = dom.whole((...locDom.myBlock.getIndices())) - wholeLow;
    for param i in 1..rank {
      var dim = zeroedLocalPart.dim(i);
      var wholestride = dom.whole.dim(i).stride;
      if dim.high >= dim.low then
        result(i) = (dim.low / wholestride)..(dim.high / wholestride) by (dim.stride / wholestride);
      else
        result(i) = 1..0 by 1;
    }
    if numTasks == 1 {
      if debugCyclicDist then
        writeln(here.id, ": leader whole: ", dom.whole,
                         " result: ", result,
                         " myblock: ", locDom.myBlock);
      yield result;
    } else {
      var splitRanges: rank*range(eltType=idxType, stridable=true) = result;

      coforall taskid in 0..#numTasks {
        const low = result(1).low, high = result(1).high;
        const (lo,hi) = _computeBlock(low, high - low + 1,
                                      low, high,
                                      numTasks, taskid);
        splitRanges(1) = result(1)(lo..hi);
        if debugCyclicDist then
          writeln(here.id, ": leader whole: ", dom.whole,
                           " result: ", result,
                           " splitRanges: ", splitRanges);
        yield splitRanges;
      }
    }
  }
}

def CyclicArr.supportsAlignedFollower() param return true;

def CyclicArr.these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
  var t: rank*range(eltType=idxType, stridable=true);
  for param i in 1..rank {
    const wholestride = dom.whole.dim(i).stride;
    t(i) = ((follower(i).low*wholestride)..(follower(i).high*wholestride) by (follower(i).stride*wholestride)) + dom.whole.dim(i).low;
  }
  const followThis = [(...t)];
  const arrSection = locArr(dom.dist.ind2locInd(followThis.low));
  if aligned {
    local {
      for i in followThis {
        yield arrSection.this(i);
      }
    }
  } else {
    def accessHelper(i) var {
      local {
        if myLocArr.locDom.myBlock.member(i) then
          return myLocArr.this(i);
      }
      return this(i);
    }

    for i in followThis {
      yield accessHelper(i);
    }
  }
}

def CyclicArr.writeThis(f: Writer) {
  if verboseCyclicDistWriters {
    writeln(typeToString(this.type));
    writeln("------");
  }
  for i in dom {
    writeln(this(i));
  }
}


class LocCyclicArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  const locDom: LocCyclicDom(rank, idxType, stridable);

  var myElems: [locDom.myBlock] eltType;

}


def LocCyclicArr.this(i:idxType) var {
  return myElems(i);
}

def LocCyclicArr.this(i:rank*idxType) var {
  return myElems((...i));
}

def LocCyclicArr.these() var {
  for elem in myElems {
    yield elem;
  }
}
