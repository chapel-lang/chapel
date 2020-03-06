class CMODist : BaseDist {
  proc dsiNewRectangularDom(param rank: int, type dimensional_index_type, param stridable: bool, inds) {
    const dom = new CMODom(rank=rank, idxType=dimensional_index_type, stridable=stridable, dist=this);
    dom.dsiSetIndices(inds);
    return dom;
  }

  proc dsiClone() return this;
}

class CMODom: BaseRectangularDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: CMODist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  proc dsiGetIndices() return ranges;

  proc dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).idxType != x(1).idxType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    chpl_assignDomainWithGetSetIndices(this, rhs);
  }

  proc dsiMyDist() return dist;

  proc dsiDims() {
    return ranges;
  }

  iter these_help(param dim: int) {
    if dim == rank - 1 {
      for j in ranges(rank) do
        for i in ranges(dim) do
          yield (i, j);
    } else {
      for j in these_help(dim+1) do
        for i in ranges(dim) do
          yield (i, (...j));
    }
  }

  iter these() {
    if rank == 1 {
      for i in ranges(1) do
        yield i;
    } else {
      for i in these_help(1) do
        yield i;
    }
  }

  iter these(param tag, followThis) ref where tag == iterKind.follower {
    yield followThis;
  }

  proc dsiAccess(dim : int)
    return ranges(dim);

  proc dsiMember(ind: idxType) where rank == 1 {
    if !ranges(1).contains(ind) then
      return false;
    return true;
  }

  proc dsiMember(ind: rank*idxType) {
    for param i in 1..rank do
      if !ranges(i).contains(ind(i)) then
        return false;
    return true;
  }

  proc dsiDim(d : int)
    return ranges(d);

  proc bbox(d: int) {
    const r: range(idxType,BoundedRangeType.bounded,false) = ranges(d);
    return r;
  }

  proc dsiNumIndices {
    var sum = 1:idxType;
    for param i in 1..rank do
      sum *= ranges(i).size;
    return sum;
    // WANT: return * reduce (this(1..rank).size);
  }

  proc dsiLow {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  proc dsiHigh {
    if rank == 1 {
      return ranges(1).high;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i).high;
      return result;
    }
  }

  proc dsiBuildArray(type eltType) {
    return new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  }
 
  proc buildSubdomain() 
    return new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  proc dsiRankChange(param rank: int, param stridable: bool, args) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    var d = new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dsiDim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  proc translate(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable = stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._translate(off(i));
    return x;
  }

  proc interior(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dim(i).high+1-dim(i) < dim(i)._low) ||
          (off(i) < 0) && (dim(i)._low-1-dim(i) > dim(i).high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dim(i)._interior(off(i));
    }
    return x;
  }

  proc exterior(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._exterior(off(i));
    return x;
  }

  proc expand(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(off(i));
      if (x.ranges(i)._low > x.ranges(i).high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  proc expand(off: int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(off);
    return x;
  }

  proc dsiStrideBy(str : rank*int) {
    var x = new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }
}

class CMOArr:BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  var dom: CMODom(rank=rank,idxType=idxType, stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var size: idxType;
  var D1: domain(1, idxType);
  var data: [D1] eltType;
  var noinit_data: bool = false;

  proc dsiGetBaseDom() return dom;
  
  proc computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs += blk(i) * off(i);
    }
  }

  proc postinit() {
    if noinit_data == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim).low;
      str(dim) = dom.dsiDim(dim).stride;
    }
    blk(1) = 1:idxType;
    for dim in 2..rank do
      blk(dim) = blk(dim-1) * dom.dsiDim(dim-1).size;
    computeFactoredOffs();
    size = blk(rank) * dom.dsiDim(rank).size;
    D1 = {0:idxType..#size:idxType};
    data = 0:eltType;
  }

  iter these() ref {
    for i in dom do {
      yield dsiAccess(i);
    }
  }

  iter these(param tag) where tag == iterKind.leader {
    for i in dom do
      yield i;
  }

  iter these(param tag, followThis) ref where tag == iterKind.follower {
    yield dsiAccess(followThis);
  }

  proc dsiAccess(ind : rank*idxType) ref {
    if boundsChecking then
      if !dom.dsiMember(ind) then
        halt("array index out of bounds: ", ind);
    var sum = origin;  
    if stridable {
      for param i in 1..rank do
        sum = sum + (ind(i) - off(i)) * blk(i) / str(i):idxType;
    } else {
      for param i in 1..rank do
        sum += ind(i) * blk(i);
      sum -= factoredOffs;
    }
    return data(sum); 
  }
  proc dsiAccess(ind : idxType) ref where rank == 1 {
    return dsiAccess( (ind, ));
  }

  proc dsiReindex(d: CMODom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dim(i).size != dom.dim(i).size then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new CMOArr(eltType=eltType, rank=d.rank, idxType=d.idxType, stridable=d.stridable, dom=d, noinit_data=true);
    //    was:  (eltType, rank, idxType, d.stridable, true, d, noinit_data=true);
    alias.D1 = {0:idxType..#size:idxType};
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dim(i).low;
      alias.blk(i) = (blk(i) * dom.dim(i).stride / str(i)) : d.idxType;
      alias.str(i) = d.dim(i).stride;
    }
    alias.origin = origin:d.idxType;
    alias.computeFactoredOffs();
    return alias;
  }

  proc dsiCheckSlice(d) {
    for param i in 1..rank {
      if d(i).boundedType == BoundedRangeType.bounded then
        if !dom.dim(i).contains(d(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", d(i));
    }
  }

  proc dsiSlice(d: CMODom) {
    var alias = new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, noinit_data=true);
    alias.D1 = {0:idxType..#size:idxType};
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    alias.origin = origin;
    for param i in 1..rank {
      alias.off(i) = d.dim(i).low;
      alias.origin += blk(i) *(d.dim(i).low - off(i))/str(i);
    }
    alias.computeFactoredOffs();
    return alias;
  }

  proc checkRankChange(args) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    for param i in 1..args.size do
      if isRange(args(i)) then
        if args(i).boundedType == BoundedRangeType.bounded then
          if !dom.dsiDim(i).contains(args(i)) then
            halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  proc dsiRankChange(d, param newRank: int, param newStridable: bool, irs) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;
    var alias = new CMOArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=newStridable, dom=d, noinit_data=true);
    alias.D1 = {0:idxType..#size:idxType};
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..irs.size {
      if isRange(irs(j)) {
        alias.off(i) = d.dsiDim(i).low;
        alias.origin += blk(j) * (d.dsiDim(i).low - off(j)) / str(j);
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
 


  proc dsiReallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var copy = new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d._value.stridable, dom=d._value);
      for i in _intersect(d._value, dom) do
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

  proc tupleInit(b: _tuple) {
    proc _tupleInitHelp(j, param rank: int, b: _tuple) {
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

proc CMODom.dsiSerialWrite(f) {
  f.write("[", this(1));
  for i in 2..rank do
    f.write(", ", this(i));
  f.write("]");
}

proc CMOArr.dsiSerialWrite(f) {
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank).stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:idxType) {
          i(dim) += dom.dsiDim(dim).stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

proc _intersect(a: CMODom, b: CMODom) {
  var c = new CMODom(rank=a.rank, idxType=a.idxType, stridable=a.stridable, dist=b.dist);
  for param i in 1..a.rank do
    c.ranges(i) = a.ranges(i)(b.ranges(i));
  return c;
}

proc main() {
  param n = 5;
  const D = {1..n,1..n};
  const D2: domain(2) dmapped new dmap(new CMODist()) = {1..n,1..n};
  var A: [D2] real = [ij in D2] ij(1) + (ij(2) - 1)*n;
  var B: [D] real = [ij in D] ij(2) + (ij(1) - 1)*n;

  writeln('A by columns');
  for j in 1..n {
    writeln(A(1..n,j));
  }
  writeln();
  writeln('B by rows');
  for i in 1..n {
    writeln(B(i,1..n));
  }

  writeln();
  write('A = ');
  writeln(A);
  writeln();
  write('B = ');
  writeln(B);
}
