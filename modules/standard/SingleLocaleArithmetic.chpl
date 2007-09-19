use Schedules;
use List;

class SingleLocaleDistribution {
  def buildDomain(param rank: int, type dim_type, param stridable: bool)
    return SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                        stridable=stridable, dist=this);

  def buildEnumDomain(type ind_type)
    return SingleLocaleEnumDomain(ind_type=ind_type);

  def buildDomain(type ind_type)
    return SingleLocaleAssociativeDomain(ind_type=ind_type);

  def buildSparseDomain(param rank:int, type dim_type,
                        parentDom: BaseArithmeticDomain)
    return SingleLocaleSparseDomain(rank=rank, dim_type=dim_type,
                                    parentDom=parentDom);
}

var Block = SingleLocaleDistribution();

class SingleLocaleArithmeticDomain: BaseArithmeticDomain {
  param rank : int;
  type dim_type;
  param stridable: bool;
  var dist: SingleLocaleDistribution;
  var ranges : rank*range(dim_type,bounded,stridable);

  def getIndices() return ranges;

  def setIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).eltType != x(1).eltType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  def buildOpenIntervalUpper() {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
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

  def member(ind: dim_type) where rank == 1 {
    if !_in(ranges(1), ind) then
      return false;
    return true;
  }

  def member(ind: rank*dim_type) {
    for param i in 1..rank do
      if !_in(ranges(i), ind(i)) then
        return false;
    return true;
  }

  def dim(d : int)
    return ranges(d);

  def bbox(d: int) {
    const r: range(dim_type,bounded,false) = ranges(d);
    return r;
  }

  def numIndices {
    var sum = 1:dim_type;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  def low {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*dim_type;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  def high {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*dim_type;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  def buildArray(type eltType) {
    return SingleLocaleArithmeticArray(eltType, rank, dim_type, stridable,
                                       dom=this);
  }

  def buildSubdomain()
    return SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                        stridable=stridable, dist=dist);

  def buildEmptyDomain() {
    if this != nil then
      return SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                          stridable=stridable, dist=dist);
    else
      // special case for array fields in classes; see
      // arrays/deitz/part2/test_array_in_class2
      return SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                          stridable=stridable,
                                          dist=SingleLocaleDistribution());

  }

  def translate(off: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=int,
                                         stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._translate(off(i));
    return x;
  }

  def interior(off: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=int,
                                         stridable=stridable, dist=dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dim(i)._high+1-off(i) < dim(i)._low) ||
          (off(i) < 0) && (dim(i)._low-1-off(i) > dim(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dim(i)._interior(off(i));
    }
    return x;
  }

  def exterior(off: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=int,
                                         stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._exterior(off(i));
    return x;
  }

  def expand(off: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=int,
                                         stridable=stridable, dist=dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(off(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(off: int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=int,
                                         stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(off);
    return x;
  }

  // this should eventually...
  //  ...be in the distribtion's implementation of the blocked domain
  // ...yield a subBlock of the domain per thread per locale
  def subBlocks {
    if (rank != 1) then halt("subBlocks not yet implemented for >1D domains");
    for block in BlockSchedule(this, Locale(0).numCores) {
      yield block;
    }
  }

  def strideBy(str : rank*int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                         stridable=true, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }

  def strideBy(str : int) {
    var x = SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                         stridable=true, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str;
    return x;
  }
}


class SingleLocaleArithmeticArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;
  param stridable: bool;
  param reindexed: bool = false; // may have blk(rank) != 1

  var dom : SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type,
                                         stridable=stridable);
  var off: rank*dim_type;
  var blk: rank*dim_type;
  var str: rank*int;
  var origin: dim_type;
  var factoredOffs: dim_type;
  var size : dim_type;
  var data : _ddata(eltType);
  var noinit: bool = false;

  def these() var {
    for i in dom do
      yield this(i);
  }

  def computeFactoredOffs() {
    factoredOffs = 0:dim_type;
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
    blk(rank) = 1:dim_type;
    for dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dim(dim+1).length;
    computeFactoredOffs();
    size = blk(1) * dom.dim(1).length;
    data = _ddata(eltType, size:int); // ahh!!! can't cast to int here
    data.init();
  }

  def this(ind: dim_type ...1) var where rank == 1
    return this(ind);

  def this(ind : rank*dim_type) var {
    if boundsChecking then
      if !dom.member(ind) then
        halt("array index out of bounds: ", ind);
    var sum = origin;
    if stridable {
      for param i in 1..rank do
        sum += (ind(i) - off(i)) * blk(i) / str(i):dim_type;
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
    var alias = SingleLocaleArithmeticArray(eltType, d.rank, d.dim_type,
                                            d.stridable, true, d, noinit=true);
    //    was:  (eltType, rank, dim_type, d.stridable, true, d, noinit=true);
    alias.data = data;
    alias.size = size: d.dim_type;
    for param i in 1..rank {
      alias.off(i) = d.dim(i)._low;
      alias.blk(i) = (blk(i) * dom.dim(i)._stride / str(i)) : d.dim_type;
      alias.str(i) = d.dim(i)._stride;
    }
    alias.origin = origin:d.dim_type;
    alias.computeFactoredOffs();
    return alias;
  }

  def checkSlice(d: SingleLocaleArithmeticDomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank {
      if !_in(dom.dim(i), d.dim(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", d);
      if d.dim(i)._stride % dom.dim(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def checkSlice(d) {
    if rank != d.size then
      halt("array rank change not supported");
    for param i in 1..rank {
      if d(i).boundedType == bounded then
        if !_in(dom.dim(i), d(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", d(i));
      if d(i)._stride % dom.dim(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def slice(d: SingleLocaleArithmeticDomain) {
    var alias = SingleLocaleArithmeticArray(eltType, rank, dim_type,
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

  def rankChange(param newRank: int, param newStridable: bool, irs, rs) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;
    var d = SingleLocaleArithmeticDomain(rank=newRank, dim_type=dim_type,
                                         stridable=newStridable, dist=dom.dist);
    d.setIndices(rs);
    var alias = SingleLocaleArithmeticArray(eltType, newRank, dim_type,
                                            newStridable, true, d, noinit=true);
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..irs.size {
      if isRange(irs(j)) {
        alias.off(i) = d.dim(i)._low;
        alias.origin += blk(j) * (d.dim(i)._low - off(j)) / str(j);
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      } else {
        alias.origin += blk(j) * (irs(j) - off(j)) / str(j);
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var new = SingleLocaleArithmeticArray(eltType, rank, dim_type,
                                            d._value.stridable, reindexed,
                                            d._value);
      for i in _intersect(d._value, dom) do
        new(i) = this(i);
      off = new.off;
      blk = new.blk;
      str = new.str;
      origin = new.origin;
      factoredOffs = new.factoredOffs;
      size = new.size;
      data = new.data;
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
  var i : rank*dim_type;
  for dim in 1..rank do
    i(dim) = dom.dim(dim)._low;
  label next while true {
    f.write(this(i));
    if i(rank) <= (dom.dim(rank)._high - dom.dim(rank)._stride:dim_type) {
      f.write(" ");
      i(rank) += dom.dim(rank)._stride:dim_type;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dim(dim)._high - dom.dim(dim)._stride:dim_type) {
          i(dim) += dom.dim(dim)._stride:dim_type;
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

def _intersect(a: SingleLocaleArithmeticDomain,
               b: SingleLocaleArithmeticDomain) {
  var c = SingleLocaleArithmeticDomain(rank=a.rank, dim_type=a.dim_type,
                                       stridable=a.stridable);
  for param i in 1..a.rank do
    c.ranges(i) = a.dim(i)(b.dim(i));
  return c;
}
