use DSIUtil;

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

  def dsiClone() return new Cyclic(rank=rank, idxType=idxType, other=this);
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

def Cyclic.dsiDisplayRepresentation() {
  writeln("lowIdx = ", lowIdx);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocs = ", for tl in targetLocs do tl.id);
  writeln("tasksPerLocale = ", tasksPerLocale);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

def Cyclic.dsiSupportsPrivatization() param return true;

def Cyclic.dsiGetPrivatizeData() return 0;

def Cyclic.dsiPrivatize(privatizeData) {
  return new Cyclic(rank=rank, idxType=idxType, other=this);
}

def Cyclic.dsiGetReprivatizeData() return 0;

def Cyclic.dsiReprivatize(other, reprivatizeData) {
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  lowIdx = other.lowIdx;
  tasksPerLocale = other.tasksPerLocale;
}

def Cyclic.dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  var dom = new CyclicDom(rank=rank, idxType=idxType, dist = this, stridable=stridable);
  dom.setup();
  return dom;
} 

def Cyclic.dsiCreateReindexDist(newSpace, oldSpace) {
  var newLow: rank*idxType;
  for param i in 1..rank {
    newLow(i) = newSpace(i).low - oldSpace(i).low + lowIdx(i);
  }
  var newDist = new Cyclic(rank=rank, idxType=idxType, low=newLow, targetLocales=targetLocs, tasksPerLocale=tasksPerLocale);
  return newDist;
}

//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int), int, (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int, range(int), range(int))
//
def _cyclic_matchArgsShape(type rangeType, type scalarType, args) type {
  def tuple(type t ...) type return t;
  def helper(param i: int) type {
    if i == args.size {
      if isCollapsedDimension(args(i)) then
        return tuple(scalarType);
      else
        return tuple(rangeType);
    } else {
      if isCollapsedDimension(args(i)) then
        return (scalarType, (... helper(i+1)));
      else
        return (rangeType, (... helper(i+1)));
    }
  }
  return helper(1);
}

def Cyclic.dsiCreateRankChangeDist(param newRank: int, args) {
  var collapsedDimInds: rank*idxType;
  var collapsedLocs: _cyclic_matchArgsShape(range(int), int, args);
  var newLow: newRank*idxType;

  var j: int = 1;
  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      collapsedDimInds(i) = args(i);
    else {
      newLow(j) = lowIdx(i);
      j += 1;
    }
  }
  const partialLocIdx = ind2locInd(collapsedDimInds);


  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) {
      collapsedLocs(i) = partialLocIdx(i);
} else {
      collapsedLocs(i) = targetLocDom.dim(i);
    }
  }
  var newTargetLocales = targetLocs[(...collapsedLocs)];
  return new Cyclic(rank=newRank, idxType=idxType, low=newLow, targetLocales=newTargetLocales);
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
  if rank == 1 then
    return x(1);
  else
    return x;
}

def Cyclic.dsiIndexLocale(i: idxType) where rank == 1 {
  return targetLocs(ind2locInd(i));
}

def Cyclic.dsiIndexLocale(i: rank*idxType) {
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
  if locDoms(dist.targetLocDom.low) == nil {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) do
        locDoms(localeIdx) = new LocCyclicDom(rank, idxType, stridable, dist.getChunk(whole, localeIdx));
    }
  } else {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) {
        var chunk = dist.getChunk(whole, localeIdx);
        locDoms(localeIdx).myBlock = chunk;
      }
    }
  }
}

def CyclicDom.dsiBuildArray(type eltType) {
  var arr = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

def CyclicDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
  dist.dsiDisplayRepresentation();
}

def CyclicDom.dsiLow return whole.low;

def CyclicDom.dsiHigh return whole.high;

def CyclicDom.dsiMember(i) return whole.member(i);

def CyclicDom.dsiIndexOrder(i) return whole.indexOrder(i);

def CyclicDom.dsiDims() return whole.dims();

def CyclicDom.dsiDim(d: int) return whole.dim(d);

def CyclicDom.getLocDom(localeIdx) return locDoms(localeIdx);



def CyclicDom.dsiGetIndices() {
  return whole.getIndices();
}

def CyclicDom.dsiSetIndices(x: domain) {
  whole = x;
  setup();
}

def CyclicDom.dsiSetIndices(x) {
  whole.setIndices(x);
  setup();
}

def CyclicDom.dsiSerialWrite(x: Writer) {
  if verboseCyclicDistWriters {
    x.writeln(typeToString(this.type));
    x.writeln("------");
    for loc in dist.targetLocDom {
      x.writeln("[", loc, "=", dist.targetLocs(loc), "] owns ", locDoms(loc).myBlock);
    }
  } else {
    x.write(whole);
  }
}

def CyclicDom.dsiNumIndices return whole.numIndices;

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

      coforall taskid in 0..#numTasks {
        var splitRanges: rank*range(eltType=idxType, stridable=true) = result;
        const low = result(1).low, high = result(1).high;
        const (lo,hi) = _computeBlock(high - low + 1, numTasks, taskid,
                                      high, low, low);
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

def CyclicDom.these(param tag: iterator, follower) where tag == iterator.follower {
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

def CyclicDom.dsiSupportsPrivatization() param return true;

def CyclicDom.dsiGetPrivatizeData() return 0;

def CyclicDom.dsiPrivatize(privatizeData) {
  // These two variables are actually necessary even though it looks like
  // dist and dist.pid could be passed to the primitive directly.
  var distpid = dist.pid;
  var thisdist = dist;
  var privdist = __primitive("chpl_getPrivatizedClass", thisdist, distpid);
  var c = new CyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

def CyclicDom.dsiGetReprivatizeData() return 0;

def CyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = other.whole;
}

def CyclicDom.dsiBuildArithmeticDom(param rank, type idxType,
                                    param stridable: bool,
                                    ranges: rank*range(idxType,
                                                       BoundedRangeType.bounded,
                                                       stridable)) {
  if idxType != dist.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("Cyclic domain rank does not match distribution's");

  var dom = new CyclicDom(rank=rank, idxType=idxType,
                         dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;

}

def CyclicDom.localSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}


class LocCyclicDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  var myBlock: domain(rank, idxType, true);
}

//
// Added as a performance stopgap to avoid returning a domain
//
def LocCyclicDom.member(i) return myBlock.member(i);


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

def CyclicArr.dsiSlice(d: CyclicDom) {
  var alias = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                            stridable=d.stridable, dom=d, pid=pid);
  var thisid = this.locale.uid;
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      alias.locArr[i] =
        new LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                         stridable=d.stridable, locDom=d.locDoms[i],
                         myElems=>locArr[i].myElems[d.locDoms[i].myBlock]);
      if thisid == here.uid then
        alias.myLocArr = alias.locArr[i];
    }
  }
  return alias;
}

def CyclicArr.dsiRankChange(d, param newRank: int, param stridable: bool, args) {
  var alias = new CyclicArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=stridable, dom = d);
  var thisid = this.locale.uid;
  coforall ind in d.dist.targetLocDom {
    on d.dist.targetLocs(ind) {
      const locDom = d.getLocDom(ind);
      var collapsedDims: rank*idxType;
      var locSlice: _cyclic_matchArgsShape(range(eltType=idxType, stridable=true), idxType, args);
      var locArrInd: rank*int;
      var j = 1;
      for param i in 1..args.size {
        if isCollapsedDimension(args(i)) {
          locSlice(i) = args(i);
          collapsedDims(i) = args(i);
        } else {
          locSlice(i) = locDom.myBlock.dim(j)(args(i));
          j += 1;
        }
      }
      locArrInd = dom.dist.ind2locInd(collapsedDims);
      j = 1;
      // Now that the locArrInd values are known for the collapsed dimensions
      // Pull the rest of the dimensions values from ind
      for param i in 1..args.size {
        if !isCollapsedDimension(args(i)) {
          if newRank > 1 then
            locArrInd(i) = ind(j);
          else
            locArrInd(i) = ind;
          j += 1;
        }
      }
      alias.locArr[ind] =
        new LocCyclicArr(eltType=eltType, rank=newRank, idxType=d.idxType,
                         stridable=d.stridable, locDom=locDom,
                         myElems=>locArr[(...locArrInd)].myElems[(...locSlice)]);
      if here.uid == thisid then
        alias.myLocArr = alias.locArr[ind];
    }
  }
  return alias;
}

def CyclicArr.dsiReindex(d: CyclicDom) {
  var alias = new CyclicArr(eltType=eltType, rank=rank, idxType=d.idxType,
                            stridable=d.stridable, dom=d);
  var thisid = this.locale.uid;
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      const locDom = d.getLocDom(i);
      const locAlias: [locDom.myBlock] => locArr[i].myElems;
      alias.locArr[i] = new LocCyclicArr(eltType=eltType, idxType=idxType,
                                         locDom=locDom, stridable=d.stridable,
                                         rank=rank, myElems=>locAlias);
      if thisid == here.uid then
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

  var A => locArr(dom.dist.ind2locInd(low)).myElems((...ranges));
  return A;
}

def CyclicArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom do
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
  dom.dsiDisplayRepresentation();
}

def CyclicArr.dsiGetBaseDom() return dom;

def CyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      locArr(localeIdx) = new LocCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

def CyclicArr.dsiSupportsPrivatization() param return true;

def CyclicArr.dsiGetPrivatizeData() return 0;

def CyclicArr.dsiPrivatize(privatizeData) {
  // These two variables are actually necessary even though it looks like
  // dist and dist.pid could be passed to the primitive directly.
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

def CyclicArr.dsiAccess(i: idxType) var where rank == 1 {
  local {
    if myLocArr != nil && myLocArr.locDom.myBlock.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def CyclicArr.dsiAccess(i:rank*idxType) var {
  return locArr(dom.dist.ind2locInd(i))(i);
}

def CyclicArr.these() var {
  for i in dom do
    yield dsiAccess(i);
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

      coforall taskid in 0..#numTasks {
        var splitRanges: rank*range(eltType=idxType, stridable=true) = result;
        const low = result(1).low, high = result(1).high;
        const (lo,hi) = _computeBlock(high - low + 1, numTasks, taskid,
                                      high, low, low);
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

def CyclicArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

def CyclicArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain.dist._value == this.dom.dist;

def CyclicArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead.dist._value == this.dom.dist;

def CyclicArr.these(param tag: iterator, follower, param fast: bool = false) var where tag == iterator.follower {
  var t: rank*range(eltType=idxType, stridable=true);
  for param i in 1..rank {
    const wholestride = dom.whole.dim(i).stride;
    t(i) = ((follower(i).low*wholestride)..(follower(i).high*wholestride) by (follower(i).stride*wholestride)) + dom.whole.dim(i).low;
  }
  const followThis = [(...t)];
  const arrSection = locArr(dom.dist.ind2locInd(followThis.low));
  if fast {
    local {
      for i in followThis {
        yield arrSection.this(i);
      }
    }
  } else {
    def accessHelper(i) var {
      local {
        if myLocArr.locDom.member(i) then
          return myLocArr.this(i);
      }
      return dsiAccess(i);
    }

    for i in followThis {
      yield accessHelper(i);
    }
  }
}

def CyclicArr.dsiSerialWrite(f: Writer) {
  if verboseCyclicDistWriters {
    writeln(typeToString(this.type));
    writeln("------");
  }
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim)._low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank)._high - dom.dsiDim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim)._high - dom.dsiDim(dim)._stride:idxType) {
          i(dim) += dom.dsiDim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

def CyclicArr.dsiReallocate(d: domain) {
  // The reallocation happens when the LocCyclicDom.myBlock field is changed
  // in CyclicDom.setup(). Nothing more needs to happen here.
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
