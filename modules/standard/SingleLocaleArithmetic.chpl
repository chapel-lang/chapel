use Schedules;
use List;

class SingleLocaleDistribution {
  def buildDomain(param rank: int, type idxType, param stridable: bool)
    return new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                            stridable=stridable, dist=this);

  def buildEnumDomain(type idxType)
    return new SingleLocaleEnumDomain(rank=1, idxType=idxType);

  def buildDomain(type idxType)
    return new SingleLocaleAssociativeDomain(rank=1, idxType=idxType);

  def buildSparseDomain(param rank:int, type idxType,
                        parentDom: BaseArithmeticDomain)
    return new SingleLocaleSparseDomain(rank=rank, idxType=idxType,
                                        parentDom=parentDom);

  def buildOpaqueDomain()
    return new SingleLocaleOpaqueDomain(rank=1);
}

var Block = new SingleLocaleDistribution();

class SingleLocaleArithmeticDomain: BaseArithmeticDomain {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: SingleLocaleDistribution;
  var ranges : rank*range(idxType,bounded,stridable);

  def getIndices() return ranges;

  def setIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).eltType != x(1).eltType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  def buildOpenIntervalUpper() {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                             stridable=stridable, dist=dist);
    for param i in 1..rank {
      if ranges(i)._stride != 1 then
        halt("syntax [domain-specification) requires a stride of one");
      x.ranges(i) = ranges(i)._low..ranges(i)._high-1;
    }
    return x;
  }

  def these_help(param d: int) {
    if d == rank - 1 {
      for i in ranges(d) do
        for j in ranges(rank) do
          yield (i, j);
    } else {
      for i in ranges(d) do
        for j in these_help(d+1) do
          yield (i, (...j));
    }
  }

  def these() {
    if rank == 1 {
      for i in ranges(1) do
        yield i;
    } else {
      for i in these_help(1) do
        yield i;
    }
  }

  def member(ind: idxType) where rank == 1 {
    if !ranges(1).member(ind) then
      return false;
    return true;
  }

  def member(ind: rank*idxType) {
    for param i in 1..rank do
      if !ranges(i).member(ind(i)) then
        return false;
    return true;
  }

  def dim(d : int)
    return ranges(d);

  def bbox(d: int) {
    const r: range(idxType,bounded,false) = ranges(d);
    return r;
  }

  def numIndices {
    var sum = 1:idxType;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  def low {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  def high {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  def buildArray(type eltType) {
    return new SingleLocaleArithmeticArray(eltType, rank, idxType, stridable,
                                           dom=this);
  }

  def buildSubdomain()
    return new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                            stridable=stridable, dist=dist);

  def buildEmptyDomain() {
    if this != nil then
      return new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                              stridable=stridable, dist=dist);
    else
      // special case for array fields in classes; see
      // arrays/deitz/part2/test_array_in_class2
      return new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                              stridable=stridable,
                                              dist=new SingleLocaleDistribution());

  }

  def slice(param stridable: bool, ranges) {
    var d = new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                             stridable=stridable, dist=dist);
    for param i in 1..rank do
      d.ranges(i) = dim(i)(ranges(i));
    return d;
  }

  def rankChange(param rank: int, param stridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var d = new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                             stridable=stridable, dist=dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  def translate(off: rank*int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=int,
                                             stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i).translate(off(i));
    return x;
  }

  def interior(off: rank*int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=int,
                                             stridable=stridable, dist=dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dim(i)._high+1-off(i) < dim(i)._low) ||
          (off(i) < 0) && (dim(i)._low-1-off(i) > dim(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dim(i).interior(off(i));
    }
    return x;
  }

  def exterior(off: rank*int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=int,
                                             stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i).exterior(off(i));
    return x;
  }

  def expand(off: rank*int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=int,
                                             stridable=stridable, dist=dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i).expand(off(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(off: int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=int,
                                             stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i).expand(off);
    return x;
  }

  // this should eventually...
  //  ...be in the distribtion's implementation of the blocked domain
  // ...yield a subBlock of the domain per thread per locale
  def subBlocks {
    if (rank != 1) then halt("subBlocks not yet implemented for >1D domains");
    for block in BlockSchedule(this, Locales(0).numCores) {
      yield block;
    }
  }

  def strideBy(str : rank*int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                             stridable=true, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }

  def strideBy(str : int) {
    var x = new SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                             stridable=true, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str;
    return x;
  }
}


class SingleLocaleArithmeticArray: BaseArray {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;
  param reindexed: bool = false; // may have blk(rank) != 1

  var dom : SingleLocaleArithmeticDomain(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var size : idxType;
  var data : _ddata(eltType);
  var noinit: bool = false;

  def these() var {
    for i in dom do
      yield this(i);
  }

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dim(dim)._low;
      str(dim) = dom.dim(dim)._stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dim(dim+1).length;
    computeFactoredOffs();
    size = blk(1) * dom.dim(1).length;
    data = new _ddata(eltType, size:int); // ahh!!! can't cast to int here
    data.init();
  }

  pragma "inline"
  def this(ind: idxType ...1) var where rank == 1
    return this(ind);

  pragma "inline"
  def this(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.member(ind) then
        halt("array index out of bounds: ", ind);
    var sum = origin;
    if stridable {
      for param i in 1..rank do
        sum += (ind(i) - off(i)) * blk(i) / str(i):idxType;
    } else {
      if reindexed {
        for param i in 1..rank do
          sum += ind(i) * blk(i);
      } else {
        for param i in 1..rank-1 do
          sum += ind(i) * blk(i);
        sum += ind(rank);
      }
      sum -= factoredOffs;
    }
    return data(sum:int); // !!ahh
  }

  def reindex(d: SingleLocaleArithmeticDomain) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dim(i).length != dom.dim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new SingleLocaleArithmeticArray(eltType, d.rank, d.idxType,
                                                d.stridable, true, d,
                                                noinit=true);
    //    was:  (eltType, rank, idxType, d.stridable, true, d, noinit=true);
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dim(i)._low;
      alias.blk(i) = (blk(i) * dom.dim(i)._stride / str(i)) : d.idxType;
      alias.str(i) = d.dim(i)._stride;
    }
    alias.origin = origin:d.idxType;
    alias.computeFactoredOffs();
    return alias;
  }

  def checkSlice(ranges) {
    for param i in 1..rank do
      if !dom.dim(i).boundsCheck(ranges(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
  }

  def slice(d: SingleLocaleArithmeticDomain) {
    var alias = new SingleLocaleArithmeticArray(eltType, rank, idxType,
                                                d.stridable, reindexed,
                                                d, noinit=true);
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    alias.origin = origin;
    for param i in 1..rank {
      alias.off(i) = d.dim(i)._low;
      alias.origin += blk(i) * (d.dim(i)._low - off(i)) / str(i);
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def checkRankChange(args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    for param i in 1..args.size do
      if isRange(args(i)) then
        if !dom.dim(i).boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  def rankChange(param newRank: int, param newStridable: bool, args) {
    var d = dom.rankChange(newRank, newStridable, args);

    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new SingleLocaleArithmeticArray(eltType, newRank, idxType,
                                                newStridable, true, d,
                                                noinit=true);
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..args.size {
      if isRange(args(j)) {
        alias.off(i) = d.dim(i)._low;
        alias.origin += blk(j) * (d.dim(i)._low - off(j)) / str(j);
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      } else {
        alias.origin += blk(j) * (args(j) - off(j)) / str(j);
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var copy = new SingleLocaleArithmeticArray(eltType, rank, idxType,
                                                 d._value.stridable, reindexed,
                                                 d._value);
      for i in d((...dom.ranges)) do
        copy(i) = this(i);
      off = copy.off;
      blk = copy.blk;
      str = copy.str;
      origin = copy.origin;
      factoredOffs = copy.factoredOffs;
      size = copy.size;
      data = copy.data;
    } else {
      halt("illegal reallocation");
    }
  }

  def tupleInit(b: _tuple) {
    def _tupleInitHelp(j, param rank: int, b: _tuple) {
      if rank == 1 {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dim(this.rank-rank+1).low + i - 1;
          this(j) = b(i);
        }
      } else {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dim(this.rank-rank+1).low + i - 1;
          _tupleInitHelp(j, rank-1, b(i));
        }
      }
    }

    if rank == 1 {
      for param i in 1..b.size do
        this(this.dom.dim(1).low + i - 1) = b(i);
    } else {
      var j: rank*int;
      _tupleInitHelp(this, j, rank, b);
    }
  }
}

def SingleLocaleArithmeticDomain.writeThis(f: Writer) {
  f.write("[", dim(1));
  for i in 2..rank do
    f.write(", ", dim(i));
  f.write("]");
}

def SingleLocaleArithmeticArray.writeThis(f: Writer) {
  if dom.numIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dim(dim)._low;
  label next while true {
    f.write(this(i));
    if i(rank) <= (dom.dim(rank)._high - dom.dim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dim(dim)._high - dom.dim(dim)._stride:idxType) {
          i(dim) += dom.dim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}
