/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// The SparseBlock distribution is defined with six classes:
//
//   Block             : (from BlockDist) distribution class
//   SparseBlockDom    : domain class
//   SparseBlockArr    : array class
//   LocSparseBlockDom : local domain class (per-locale instances)
//   LocSparseBlockArr : local array class (per-locale instances)
//
// When a distribution, domain, or array class instance is created, a
// corresponding local class instance is created on each locale that is
// mapped to by the distribution.
//

use DSIUtil;
use ChapelUtil;
use BlockDist;
//
// These flags are used to output debug information and run extra
// checks when using SparseBlock.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugSparseBlockDist = false;
config param debugSparseBlockDistBulkTransfer = false;
// There is no SparseBlock distribution class. Instead, we
// just use Block.

//
// SparseBlock Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class SparseBlockDom: BaseSparseDomImpl {
  type sparseLayoutType;
  param stridable: bool = false;  // TODO: remove default value eventually
  const dist: Block(rank, idxType, sparseLayoutType);
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  var locDoms: [dist.targetLocDom] LocSparseBlockDom(rank, idxType, stridable,
      sparseLayoutType);

  proc initialize() {
    setup();
    //    writeln("Exiting initialize");
  }

  proc setup() {
    //    writeln("In setup");
    if locDoms(dist.targetLocDom.low) == nil {
      coforall localeIdx in dist.targetLocDom do {
        on dist.targetLocales(localeIdx) do {
          //                    writeln("Setting up on ", here.id);
          //                    writeln("setting up on ", localeIdx, ", whole is: ", whole, ", chunk is: ", dist.getChunk(whole,localeIdx));
         locDoms(localeIdx) = new LocSparseBlockDom(rank, idxType, stridable,
             sparseLayoutType, dist.getChunk(whole,localeIdx));
          //                    writeln("Back on ", here.id);
        }
      }
      //      writeln("Past coforall");
    } else {
      halt("Don't know how to reallocate yet");
      /*
      coforall localeIdx in dist.targetLocDom do {
        on dist.targetLocales(localeIdx) do
          locDoms(localeIdx).mySparseBlock = dist.getChunk(whole, localeIdx);
      }
      */
    }
    //    writeln("Exiting setup()");
  }

  proc dsiDestroyDom() {
    coforall localeIdx in dist.targetLocDom do {
      on locDoms(localeIdx) do
        delete locDoms(localeIdx);
    }
  }

  // TODO: For some reason I have to make all the methods for these classes primary
  // rather than secondary methods.  This doesn't seem right, but I couldn't boil
  // it down to a smaller test case in the time I spent on it.
  proc dsiAdd(ind: rank*idxType) {
    var _retval = 0;
    on dist.dsiIndexToLocale(ind) {
      _retval = locDoms[dist.targetLocsIdx(ind)].dsiAdd(ind);
    }
    nnz += _retval;
    return _retval;
  }

  proc dsiAdd(ind: idxType) where this.rank == 1 {
    return dsiAdd((ind,));
  }

  proc dsiFirst {
    return min reduce ([l in locDoms] l.mySparseBlock.first);
  }

  proc dsiLast {
    return max reduce ([l in locDoms] l.mySparseBlock.last);
  }

  // Tried to put this record in the function and the if statement, but got a
  // segfault from the compiler.
  record TargetLocaleComparator {
    proc key(a: index(rank, idxType)) { 
      return (dist.targetLocsIdx(a), a);
    }
  }

  proc bulkAdd_help(inds: [] index(rank,idxType),
      dataSorted=false, isUnique=false) {
    use Sort;
    use Search;

    // without _new_, record functions throw null deref
    var comp = new TargetLocaleComparator();

    if !dataSorted then sort(inds, comparator=comp);

    var localeRanges: [dist.targetLocDom] range;
    on inds {
      for l in dist.targetLocDom {
        const _first = locDoms[l].mySparseBlock._value.parentDom.first;
        const _last = locDoms[l].mySparseBlock._value.parentDom.last;

        var (foundFirst, locFirst) = binarySearch(inds, _first, comp);
        var (foundLast, locLast) = binarySearch(inds, _last, comp);

        if !foundLast then locLast -= 1;

        // two ifs are necessary to catch out of bounds in the bulkAdd call
        // chain. otherwise this methods would cutoff indices that are smaller
        // than parentDom.first or larger than parentDom.last, which is
        // _probably_ not desirable.
        if dist.targetLocDom.first == l then
          locFirst = inds.domain.first;
        if dist.targetLocDom.last == l then
          locLast = inds.domain.last;

        localeRanges[l] = locFirst..locLast;
      }
    }
    var _totalAdded: atomic int;
    coforall l in dist.targetLocDom do on dist.targetLocales[l] {
      const _retval = locDoms[l].mySparseBlock.bulkAdd(inds[localeRanges[l]],
          dataSorted=true, isUnique=false);
      _totalAdded.add(_retval);
    }
    const _retval = _totalAdded.read();
    nnz += _retval;
    return _retval;
  }

  //
  // output domain
  //
  proc dsiSerialWrite(f) {
    if (rank == 1) {
      f <~> "{";
      for locdom in locDoms do {
        // on locdom do {
        if (locdom.dsiNumIndices) {
            f <~> " ";
            locdom.dsiSerialWrite(f);
          }
          //}
      }
      f <~> "}";
    } else {
      compilerError("Can't write out multidimensional sparse distributed domains yet");
    }
  }

  //
  // how to allocate a new array over this domain
  //
  proc dsiBuildArray(type eltType) {
    var arr = new SparseBlockArr(eltType=eltType, rank=rank, idxType=idxType,
        stridable=stridable, sparseLayoutType=sparseLayoutType, dom=this);
    arr.setup();
    return arr;
  }

  // stopgap to avoid accessing locDoms field (and returning an array)
  proc getLocDom(localeIdx) return locDoms(localeIdx);

  iter these() {
    for locDom in locDoms do
      // TODO Would want to do something like:
      //on blk do
      // But can't currently have yields in on clauses:
      // invalid use of 'yield' within 'on' in serial iterator
      for x in locDom.mySparseBlock._value.these() do
        yield x;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    coforall (locDom,localeIndex) in zip(locDoms,dist.targetLocDom) {
      on locDom {
        for followThis in locDom.mySparseBlock._value.these(tag) {
          yield (followThis, localeIndex);
        }
      }
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    var (locFollowThis, localeIndex) = followThis;
    for i in locFollowThis(1).these(tag, locFollowThis) do
      yield i;
  }

  iter these(param tag: iterKind) where tag == iterKind.standalone &&
    // Ensure it is legal to invoke the standalone iterator
    // on locDom.mySparseBlock below.
    __primitive("method call resolves",
                locDoms[createTuple(rank,int,0)].mySparseBlock._value,
                "these", tag)
  {
    coforall locDom in locDoms {
      on locDom {
        forall i in locDom.mySparseBlock {
          yield i;
        }
      }
    }
  }

  proc dsiMember(ind) {
    on whole.dist.idxToLocale(ind) {
      writeln("Need to add support for mapping locale to local domain");
    }
  }

  proc dsiClear() {
    nnz = 0;
    coforall locDom in locDoms do
      on locDom do
        locDom.dsiClear();
  }

  proc dsiMyDist() return dist;

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    if !lhsPrivate then
      halt("SparseBlock domain assignment not yet supported");
    for i in rhs do
      dsiAdd(i);
  }

}

//
// Local SparseBlock Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// mySparseBlock: a non-distributed domain that defines the local indices
//
class LocSparseBlockDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  type sparseLayoutType;
  var parentDom: domain(rank, idxType, stridable);
  var sparseDist = new sparseLayoutType; //unresolved call workaround
  var mySparseBlock: sparse subdomain(parentDom) dmapped new dmap(sparseDist);

  proc dsiAdd(ind: rank*idxType) {
    return mySparseBlock.add(ind);
  }

  proc dsiMember(ind: rank*idxType) {
    return mySparseBlock.member(ind);
  }

  proc dsiClear() {
    mySparseBlock.clear();
  }

  proc dsiSerialWrite(w) {
    mySparseBlock._value.dsiSerialWrite(w, printBrackets=false);
    // w.write(mySparseBlock); // works, but gets brackets printed out redundantly
    //    w <~> mySparseBlock;
  }

  proc dsiNumIndices {
    return mySparseBlock.numIndices;
  }
}

//
// SparseBlock Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class SparseBlockArr: BaseSparseArr {
  param stridable: bool;
  type sparseLayoutType = DefaultDist;

  // ideally I wanted to have `var locArr: [dom.dist.targetLocDom]`. However,
  // superclass' fields cannot be used in child class' field initializers. See
  // the constructor for the workaround.
  var locArrDom: domain(rank,idxType);
  var locArr: [locArrDom] LocSparseBlockArr(eltType, rank, idxType, stridable,
      sparseLayoutType);
  var myLocArr: LocSparseBlockArr(eltType, rank, idxType, stridable,
      sparseLayoutType);

  proc SparseBlockArr(type eltType, param rank, type idxType, param stridable,
      type sparseLayoutType ,dom) {
    locArrDom = dom.dist.targetLocDom;
  }

  proc setup() {
    var thisid = this.locale.id;
    coforall localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocales(localeIdx) {
        const locDom = dom.getLocDom(localeIdx);
        locArr(localeIdx) = new LocSparseBlockArr(eltType, rank, idxType,
            stridable, sparseLayoutType, locDom);
        if thisid == here.id then
          myLocArr = locArr(localeIdx);
      }
    }
  }

  proc dsiDestroyArr() {
    coforall localeIdx in dom.dist.targetLocDom {
      on locArr(localeIdx) {
        delete locArr(localeIdx);
      }
    }
  }

  iter these() ref {
    for locI in dom.dist.targetLocDom {
      // TODO Would want to do something like:
      //on locDom do
      // But can't currently have yields in on clauses:
      // invalid use of 'yield' within 'on' in serial iterator
      var locDom = dom.locDoms[locI];
      var locArrI = locArr[locI];
      for x in locDom.mySparseBlock {
        yield locArrI.myElems(x);
      }
    }
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    for followThis in dom.these(tag) do
      yield followThis;
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    var (locFollowThis, localeIndex) = followThis;
    for i in locFollowThis(1).these(tag, locFollowThis) {
      yield locArr[localeIndex].dsiAccess(i);
    }
  }

  iter these(param tag: iterKind) ref where tag == iterKind.standalone &&
    // Ensure it is legal to invoke the standalone iterator
    // on locA.myElems below.
    __primitive("method call resolves",
                locArr[locArrDom.low].myElems._value,
                "these", tag)
   {
    coforall locA in locArr do on locA {
      // forward to sparse standalone iterator
      forall i in locA.myElems {
        yield i;
      }
    }
  }


  proc dsiAccess(i: rank*idxType) ref {
    local {
      if myLocArr != nil && myLocArr.locDom.parentDom.member(i) {
        return myLocArr.dsiAccess(i);
      }
    }
    return locArr[dom.dist.targetLocsIdx(i)].dsiAccess(i);
  }
  proc dsiAccess(i: rank*idxType)
  where shouldReturnRvalueByValue(eltType) {
    local {
      if myLocArr != nil && myLocArr.locDom.parentDom.member(i) {
        return myLocArr.dsiAccess(i);
      }
    }
    return locArr[dom.dist.targetLocsIdx(i)].dsiAccess(i);
  }
  proc dsiAccess(i: rank*idxType) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    local {
      if myLocArr != nil && myLocArr.locDom.parentDom.member(i) {
        return myLocArr.dsiAccess(i);
      }
    }
    return locArr[dom.dist.targetLocsIdx(i)].dsiAccess(i);
  }




  proc dsiAccess(i: idxType...rank) ref
    return dsiAccess(i);
  proc dsiAccess(i: idxType...rank)
  where shouldReturnRvalueByValue(eltType)
    return dsiAccess(i);
  proc dsiAccess(i: idxType...rank) const ref
  where shouldReturnRvalueByConstRef(eltType)
    return dsiAccess(i);



  proc dsiGetBaseDom() return dom;

}

//
// Local SparseBlock Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocSparseBlockArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  type sparseLayoutType;
  const locDom: LocSparseBlockDom(rank, idxType, stridable, sparseLayoutType);
  var myElems: [locDom.mySparseBlock] eltType;

  proc dsiAccess(i) ref {
    return myElems[i];
  }
  proc dsiAccess(i)
  where shouldReturnRvalueByValue(eltType) {
    return myElems[i];
  }
  proc dsiAccess(i) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    return myElems[i];
  }
}

/*

Some old code that might be useful to draw from as this
module is improved.

proc SparseBlockDom.dsiNewSpsSubDom(parentDomVal) {
  return new SparseBlockDom(rank, idxType, dist, parentDomVal);
}

proc SparseBlockDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].mySparseBlock = ", locDoms[tli].mySparseBlock);
}



//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int(32)), int(32), (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int(32), range(int(32)), range(int(32)))
//
proc _matchArgsShape(type rangeType, type scalarType, args) type {
  proc tuple(type t ...) type return t;
  proc helper(param i: int) type {
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


proc SparseBlockDom.dsiLow return whole.low;
proc SparseBlockDom.dsiHigh return whole.high;
proc SparseBlockDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for a rectangular
// domain take a domain rather than something else?
//
proc SparseBlockDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
  if debugSparseBlockDist {
    writeln("Setting indices of SparseBlock domain:");
    dsiDisplayRepresentation();
  }
}

proc SparseBlockDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
  if debugSparseBlockDist {
    writeln("Setting indices of SparseBlock domain:");
    dsiDisplayRepresentation();
  }
}

proc SparseBlockDom.dsiGetIndices() {
  return whole.getIndices();
}

// dsiLocalSlice
proc SparseBlockDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

proc SparseBlockDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocSparseBlockDom.member(i) return mySparseBlock.member(i);

proc SparseBlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
  }
}

inline proc _remoteAccessData.getDataIndex(param stridable, ind: rank*idxType) {
  // modified from DefaultRectangularArr below
  var sum = origin;
  if stridable {
    for param i in 1..rank do
      sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
  } else {
    for param i in 1..rank do
      sum += ind(i) * blk(i);
    sum -= factoredOffs;
  }
  return sum;
}

proc SparseBlockArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }
  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idx(j);
      j += 1;
    }
  }
  return tup;
}

proc _extendTuple(type t, idx, args) {
  var tup: args.size*t;
  var idxTup = tuple(idx);
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}

proc SparseBlockArr.dsiReallocate(d: domain) {
  //
  // For the default rectangular array, this function changes the data
  // vector in the array class so that it is setup once the default
  // rectangular domain is changed.  For this distributed array class,
  // we don't need to do anything, because changing the domain will
  // change the domain in the local array class which will change the
  // data in the local array class.  This will only work if the domain
  // we are reallocating to has the same distribution, but domain
  // assignment is defined so that only the indices are transferred.
  // The distribution remains unchanged.
  //
}

proc SparseBlockArr.dsiPostReallocate() {
  // Call this *after* the domain has been reallocated
}

//
// the accessor for the local array -- assumes the index is local
//
proc LocSparseBlockArr.this(i) ref {
  return myElems(i);
}

*/

//
// output array
//
proc SparseBlockArr.dsiSerialWrite(f) {
  if (rank == 1) {
    f <~> "[";
    for locarr in locArr do {
      // on locdom do {
      if (locarr.locDom.dsiNumIndices) {
        f <~> " ";
        locarr.dsiSerialWrite(f);
      }
      // }
    }
    f <~> "]";
  } else {
    compilerError("Can't write out multidimensional sparse distributed arrays yet");
  }
}

proc LocSparseBlockArr.dsiSerialWrite(f) {
  myElems._value.dsiSerialWrite(f);
}


proc SparseBlockDom.dsiSupportsPrivatization() param return true;

proc SparseBlockDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc SparseBlockDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(1));
  var c = new SparseBlockDom(rank=rank, idxType=idxType,
                             sparseLayoutType=sparseLayoutType,
                             stridable=parentDom.stridable, dist=privdist,
                             whole=whole,
                             parentDom=parentDom);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = {(...privatizeData(2))};
  return c;
}

proc SparseBlockDom.dsiGetReprivatizeData() return whole.dims();

proc SparseBlockDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = {(...reprivatizeData)};
}

proc SparseBlockArr.dsiSupportsPrivatization() param return true;

proc SparseBlockArr.dsiGetPrivatizeData() return dom.pid;

proc SparseBlockArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);
  var c = new SparseBlockArr(sparseLayoutType=sparseLayoutType,
      eltType=eltType, rank=rank, idxType=idxType, stridable=stridable,
      dom=privdom);
  for localeIdx in c.dom.dist.targetLocDom {
    c.locArr(localeIdx) = locArr(localeIdx);
    if c.locArr(localeIdx).locale.id == here.id then
      c.myLocArr = c.locArr(localeIdx);
  }
  return c;
}

// TODO: bulk-transfer support

iter ConsecutiveChunks(d1,d2,lid,lo) {
  var elemsToGet = d1.locDoms[lid].mySparseBlock.numIndices;
  const offset   = d2.whole.low - d1.whole.low;
  var rlo=lo+offset;
  var rid  = d2.dist.targetLocsIdx(rlo);
  while (elemsToGet>0) {
    const size = min(d2.numRemoteElems(rlo,rid),elemsToGet):int;
    yield (rid,rlo,size);
    rid +=1;
    rlo += size;
    elemsToGet -= size;
  }
}

iter ConsecutiveChunksD(d1,d2,i,lo) {
  const rank=d1.rank;
  var elemsToGet = d1.locDoms[i].mySparseBlock.dim(rank).length;
  const offset   = d2.whole.low - d1.whole.low;
  var rlo = lo+offset;
  var rid = d2.dist.targetLocsIdx(rlo);
  while (elemsToGet>0) {
    const size = min(d2.numRemoteElems(rlo(rank):int,rid(rank):int),elemsToGet);
    yield (rid,rlo,size);
    rid(rank) +=1;
    rlo(rank) += size;
    elemsToGet -= size;
  }
}

proc SparseBlockDom.numRemoteElems(rlo,rid){
  var blo,bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank).length - 1) then
    bhi=whole.dim(rank).high;
  else
      bhi=dist.boundingBox.dim(rank).low +
        intCeilXDivByY((dist.boundingBox.dim(rank).high - dist.boundingBox.dim(rank).low +1)*(rid+1),
                   dist.targetLocDom.dim(rank).length) - 1;

  return(bhi - rlo + 1);
}

//Brad's utility function. It drops from Domain D the dimensions
//indicated by the subsequent parameters dims.
proc dropDims(D: domain, dims...) {
  var r = D.dims();
  var r2: (D.rank-dims.size)*r(1).type;
  var j = 1;
  for i in 1..D.rank do
    for k in 1..dims.size do
      if dims(k) != i {
        r2(j) = r(i);
        j+=1;
      }
  var DResult = {(...r2)};
  return DResult;
}

