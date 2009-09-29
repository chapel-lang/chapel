module CyclicDistrib {
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

class CyclicDist : BaseDist {
  param rank: int = 1;
  type idxType = int(64);

  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;

  const locDist: [targetLocDom] LocCyclicDist(rank, idxType);

  const tasksPerLoc = 1;

  def CyclicDist(param rank: int,
                 type idxType = int(64),
                 targetLocales: [] locale = Locales, tasksPerLocale=1) {
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
    tasksPerLoc = tasksPerLocale;
    coforall locid in targetLocDom {
      on targetLocs(locid) {
        locDist(locid) = new LocCyclicDist(rank, idxType, locid, this);
      }
    }
  }

  def clone() return new CyclicDist(rank, idxType, targetLocs, tasksPerLoc);

  def CyclicDist(param rank: int, type idxType, other: CyclicDist(rank, idxType)) {
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    locDist = other.locDist;
    tasksPerLoc = other.tasksPerLoc;
  }

  def getChunk(inds, locid) {
    return inds((...locDist(locid).myChunk.getIndices()));
  }

  def newArithmeticDom(param rank: int, type idxType, param stridable: bool, param alias: bool = false) {
    if idxType != this.idxType then
      compilerError("Block domain index type does not match distribution's");
    if rank != this.rank then
      compilerError("Block domain rank does not match distribution's");
    var dom = new CyclicDom(rank=rank, idxType=idxType, dist = this, stridable=stridable, alias=false);
    dom.setup();
    return dom;
  } 

  def writeThis(x: Writer) {
    x.writeln("Cyclic");
    x.writeln("------");
    for locid in targetLocDom do
      x.writeln(" [", locid, "=", targetLocs(locid), "] owns chunk: ", locDist(locid).myChunk); 
  }

  def ind2locInd(i: idxType) {
    pragma "inline" def mod(a:integral, b:integral) {
      if (b <= 0) then
        halt("modulus divisor must be positive");
      var tmp = a % b:a.type;
      return if tmp < 0 then b:a.type + tmp else tmp;
    }
    const numLocs:idxType = targetLocDom.numIndices:idxType;
    return mod(mod(i, numLocs) - mod(min(idxType), numLocs), numLocs):int;
  }

  def ind2locInd(ind: rank*idxType) {
    var x: rank*idxType;
    var minIdx = min(idxType);
    for param i in 1..rank {
      var dimLen = targetLocDom.dim(i).length;
      x(i) = mod(mod(ind(i), dimLen) - mod(minIdx, dimLen), dimLen);
    }
    return x;
  }
  def ind2loc(i: idxType) where rank == 1 {
    return targetLocs(ind2locInd(i));
  }
  def ind2loc(i: rank*idxType) {
    return targetLocs(ind2locInd(i));
  }
}

class LocCyclicDist {
  param rank: int;
  type idxType;

  const myChunk: domain(rank, idxType, true);

  def LocCyclicDist(param rank, type idxType, locid, dist: CyclicDist(rank, idxType)) {
    if rank == 1 {
      const lo = min(idxType) + locid:idxType;
      const hi = max(idxType);
      myChunk = [lo..hi by dist.targetLocDom.numIndices];
    } else {
      var tuple: rank*range(idxType, stridable=true);
      for param i in 1..rank {
        tuple(i) = (min(idxType) + locid(i):idxType)..max(idxType) by dist.targetLocDom.dim(i).length;
      }
      myChunk = [(...tuple)];
    }
  }
}

class CyclicDom : BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  param alias: bool = false;

  const dist: CyclicDist(rank, idxType);

  var locDoms: [dist.targetLocDom] LocCyclicDom(rank, idxType, stridable);

  const whole: domain(rank, idxType, stridable);

  var pid: int = -1;

  def setup() {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) {
        locDoms(localeIdx) = new LocCyclicDom(rank, idxType, stridable, this,
                                              dist.getChunk(whole, localeIdx));
      }
    }
  }

  def buildArray(type eltType) {
    var arr = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this, alias=false);
    arr.setup();
    return arr;
  }

  def setIndices(x) {
    whole.setIndices(x);
    setup();
  }

  def writeThis(x: Writer) {
    for loc in dist.targetLocDom {
      x.writeln("[", loc, "=", dist.targetLocs(loc), "] owns ", locDoms(loc).myBlock);
    }
  }
  def numIndices return whole.numIndices;

  def these() {
    for i in whole do
      yield i;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    const wholeLow = whole.low,
          precomputedNumTasks = dist.tasksPerLoc;
    coforall locDom in locDoms do on locDom {
      const locBlock = locDom.myBlock - wholeLow;
      const numTasks = precomputedNumTasks;
      if numTasks == 1 {
        yield locBlock;
      } else {
        if rank == 1 {
          coforall taskid in 0..#numTasks {
            const low = locBlock.low, high = locBlock.high;
            const (lo,hi) = _computeBlock(low, high - low + 1,
                                         low, high,
                                         numTasks, taskid);
            yield locBlock[lo..hi];
          }
        } else {
          var ranges: rank*range(stridable=true);
          for param i in 2..rank do
            ranges(i) = locBlock.dim(i);
          coforall taskid in 0..#numTasks {
            const low = locBlock.dim(1).low, high = locBlock.dim(1).high;
            const (lo,hi) = _computeBlock(low, high - low + 1,
                                          low, high,
                                          numTasks, taskid);
            ranges(1) = locBlock.dim(1)(lo..hi);
            yield locBlock[(...ranges)];
          }
        }
      }
    }
  }

  def these(param tag: iterator, follower, param aligned: bool = false) where tag == iterator.follower {
    const followThis = follower + whole.low;
    for i in followThis do
      yield i;
  }

  def supportsPrivatization() param return true;
  def privatize() {
    var privateDist = new CyclicDist(rank, idxType, dist);
    var c = new CyclicDom(idxType=idxType, rank=rank, stridable=stridable, dist=privateDist);
    c.locDoms = locDoms;
    c.whole = whole;
    return c;
  }
  def reprivatize(other) {
    locDoms = other.locDoms;
    whole = other.whole;
  }
  def dim(d: int) return whole.dim(d);
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
  param alias: bool = false;
  var dom: CyclicDom(rank, idxType, stridable);

  var locArr: [dom.dist.targetLocDom] LocCyclicArr(eltType, rank, idxType, stridable);
  var pid: int = -1;

  def getBaseDom() return dom;

  def setup() {
    coforall localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocs(localeIdx) {
        locArr(localeIdx) = new LocCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      }
    }
  }

  def supportsPrivatization() param return true;
  def privatize() {
    var dompid = dom.pid;
    var thisdom = dom;
    var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
    var c = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
    c.locArr = locArr;
    return c;
  }

  def this(i: idxType) var where rank == 1 {
    return locArr(dom.dist.ind2locInd(i))(i);
  }

  def this(i:rank*idxType) var {
    var x = locArr(dom.dist.ind2locInd(i));
    return x(i);
  }

  def these() var {
    for i in dom do
      yield this(i);
  }

  def these(param tag: iterator) where tag == iterator.leader {
    const wholeLow = dom.whole.low,
          precomputedNumTasks = dom.dist.tasksPerLoc;
    coforall locDom in dom.locDoms do on locDom {
      const locBlock = locDom.myBlock - wholeLow;
      const numTasks = precomputedNumTasks;
      if numTasks == 1 {
        yield locBlock;
      } else {
        if rank == 1 {
          coforall taskid in 0..#numTasks {
            const low = locBlock.low, high = locBlock.high;
            const (lo,hi) = _computeBlock(low, high - low + 1,
                                         low, high,
                                         numTasks, taskid);
            yield locBlock[lo..hi];
          }
        } else {
          var ranges: rank*range(stridable=true);
          for param i in 2..rank do
            ranges(i) = locBlock.dim(i);
          coforall taskid in 0..#numTasks {
            const low = locBlock.dim(1).low, high = locBlock.dim(1).high;
            const (lo,hi) = _computeBlock(low, high - low + 1,
                                          low, high,
                                          numTasks, taskid);
            ranges(1) = locBlock.dim(1)(lo..hi);
            yield locBlock[(...ranges)];
          }
        }
      }
    }
  }

  def supportsAlignedFollower() param return true;

  def these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
    const followThis = follower + dom.whole.low;
    const myLocArr = locArr(dom.dist.ind2locInd(followThis.low));
    if aligned {
      local {
        for i in followThis {
          yield myLocArr.this(i);
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

  def writeThis(f: Writer) {
    for i in dom {
      writeln(this(i));
    }
  }
}

class LocCyclicArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  const locDom: LocCyclicDom(rank, idxType, stridable);

  var myElems: [locDom.myBlock] eltType;

  def this(i:idxType) var {
    return myElems(i);
  }

  def this(i:rank*idxType) var {
    return myElems((...i));
  }

  def these() var {
    for elem in myElems {
      yield elem;
    }
  }
}

config const tpl = 2;

def smain {
  const r = 3.0;
  const Dist = new CyclicDist(1, int(64), tasksPerLocale = 2);
  const Dom: domain(1, int(64)) distributed Dist = [1..10:int(64)];
  var A, B: [Dom] real;

  forall i in Dom {
    B(i) = i;
  }

  forall (a, b) in (A, B) {
    a = b * r;
  }

  writeln(A);
  writeln(B);
}

def tmain {
  var Locs = Locales;
  var Dist = new CyclicDist(2, int, Locs, tasksPerLocale=tpl);
  var Dom: domain(2, int) distributed Dist = [1..10, 1..10];
  var Arr: [Dom] real;

  writeln(Dist);
  writeln(Dom);
  writeln(Arr);
  for (i,j) in Dom {
    writeln(Arr(i,j).locale, ": Arr(", i, ",", j, ") = ", Arr(i,j));
  }

  forall (i,j) in Dom { Arr(i,j) = i*100 + j; }
  forall a in Arr { a += 1; }
  writeln(Arr);
  /*
  var Dom: domain(1, int) distributed Dist = [-10..#20];
  var Arr: [Dom] int;
  Arr(5) = 1;
  writeln(Dist);
  writeln(Dom);
  writeln(Arr);
  for i in Dom {
    writeln("Arr(", i, ").locale = ", Arr(i).locale);
  }
  for a in Arr {
    writeln("a.locale = ", a.locale);
  }
  forall a in Arr {
    writeln(a.locale);
  }
  forall i in Dom {
    writeln(here, ": Arr(", i, ") = ", Arr(i), " Arr(", i, ").locale = ", Arr(i).locale);
  }
  */
}
}
