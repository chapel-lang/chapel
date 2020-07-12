/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

// ArrayViewRankChange
//
// This module provides an array domain map class that is used to
// represent rank change slices of another array via a domain.
//
// Throughout this file, 'down*' refers to the higher-dimensional
// array/domain/distribution that's been rank-changed in this view,
// while 'up*' refers to the lower-dimensional, user-facing view.
//
// Note that because only rectangular domains have a notion of having
// rank > 1, rank-change views are only supported for rectangular
// domains and arrays.
//
// TODOs for this file:
//
// * the dist, dom, and arr classes store a certain amount of
//   duplicated information -- should these be focused in the higher
//   classes (dist/dom) and referenced by lower ones (dom/dist)?
//
// * I don't think I've done anything to privatize the dist or dom
//   classes... should I?
//
module ArrayViewRankChange {
  private use ChapelStandard;

  //
  // This class represents a distribution that knows how to create
  // rank-change domains and arrays similar to the one that caused
  // it to be created.
  //
  class ArrayViewRankChangeDist: BaseDist {
    // a pointer down to the distribution that this class is creating
    // lower-dimensional views of
    var downDistPid:int;
    var downDistInst;

    // These two fields represent whether or not each dimension was
    // collapsed as part of the rank-change; and if so, what the
    // index of that collapsed dimension was.  So for A[lo..hi, 3]
    // these would be (false, true) and (?, 3) respectively.
    //
    // As future work (TODO), note that collapsedDim ought to be able
    // to be changed into a param tuple rather than an execution-time
    // one.
    const collapsedDim;
    const idx;

    inline proc downDist {
      if _isPrivatized(downDistInst) then
        return chpl_getPrivatizedCopy(downDistInst.type, downDistPid);
      else
        return downDistInst;
    }

    override proc dsiNewRectangularDom(param rank, type idxType, param stridable, inds) {
      var newdom = new unmanaged ArrayViewRankChangeDom(rank=rank,
                                              idxType=idxType,
                                              stridable=stridable,
                                              collapsedDim=collapsedDim,
                                              idx=idx,
                                              distPid=this.pid,
                                              distInst=_to_unmanaged(this));
      newdom.dsiSetIndices(inds);
      return newdom;
    }

    proc dsiClone() {
      return new unmanaged ArrayViewRankChangeDist(downDistPid=this.downDistPid,
                                                       downDistInst=this.downDistInst,
                                                       collapsedDim=collapsedDim,
                                                       idx=idx);
    }

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    override proc dsiSupportsPrivatization() param
      return downDistInst.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (downDistPid, downDistInst, collapsedDim, idx);
    }

    proc dsiPrivatize(privatizeData) {
      return new unmanaged ArrayViewRankChangeDist(downDistPid = privatizeData(0),
                                         downDistInst = privatizeData(1),
                                         collapsedDim = privatizeData(2),
                                         idx = privatizeData(3));
    }

    override proc dsiDestroyDist() {
    }

    override proc dsiIsLayout() param {
      return downDistInst.dsiIsLayout();
    }
  }

  private proc downDomType(param rank : int,
                           type idxType,
                           param stridable : bool,
                           dist) type {
      var ranges: rank*range(idxType, BoundedRangeType.bounded, stridable);
      var a = dist.downDist.dsiNewRectangularDom(rank=rank, idxType,
                                                 stridable=stridable, ranges);
      return a.type;
  }

  //
  // This class represents the domain of a rank-change slice of an
  // array.  Like other domain class implementations, it supports the
  // standard dsi interface.  Note that rank changes only make sense
  // for rectangular domains (because they're the only ones with
  // rank>1), so this is a subclass of BaseRectangularDom.
  //
 class ArrayViewRankChangeDom: BaseRectangularDom {
    // the lower-dimensional index set that we represent upwards
    var upDomInst: unmanaged DefaultRectangularDom(rank, idxType, stridable)?;
    forwarding upDom except these;

    // the collapsed dimensions and indices in those dimensions
    //
    // Note that these are the same as in the distribution and could
    // potentially be pulled from there.
    const collapsedDim;
    const idx;

    const distPid;  // a reference back to our ArrayViewRankChangeDist
    const distInst;

    inline proc dist {
      if _isPrivatized(distInst) then
        return chpl_getPrivatizedCopy(distInst.type, distPid);
      else
        return distInst;
    }

    // the higher-dimensional domain that we're equivalent to
    //
    // BHARSH INIT TODO: use 'downrank' instead of 'collapsedDim.size'
    //
    var downDomPid:int;
    var downDomInst: downDomType(collapsedDim.size, idxType, stridable, distInst)?;

    proc downrank param {
      return collapsedDim.size;
    }

    inline proc upDom {
      return upDomInst!;
    }

    inline proc downDom {
      if _isPrivatized(downDomInst!) then
        return chpl_getPrivatizedCopy(downDomInst!.type, downDomPid);
      else
        return downDomInst!;
    }

    proc dsiBuildArray(type eltType, param initElts:bool) {
      pragma "no auto destroy"
      const downarr = _newArray(downDom.dsiBuildArray(eltType, initElts));
      return new unmanaged ArrayViewRankChangeArr(eltType  =eltType,
                                        _DomPid = this.pid,
                                        dom = _to_unmanaged(this),
                                        _ArrPid=downarr._pid,
                                        _ArrInstance=downarr._instance,
                                        collapsedDim=collapsedDim,
                                        idx=idx,
                                        ownsArrInstance=true);
    }

    proc dsiSetIndices(inds) {
      // Free underlying domains if necessary
      this.dsiDestroyDom();

      pragma "no auto destroy"
      var upDomRec = {(...inds)};
      upDomInst = upDomRec._value;

      // TODO: BHARSH 2019-05-13:
      // Would rather use '_getDistribution' and passing args to "new _domain",
      // see similar comment in ArrayViewReindex for more information.
      var ranges : downrank*range(idxType, BoundedRangeType.bounded, stridable);
      var downDomClass = dist.downDist.dsiNewRectangularDom(rank=downrank,
                                                           idxType,
                                                           stridable=stridable,
                                                           ranges);
      pragma "no auto destroy"
      var downDomLoc = new _domain(downDomClass);
      downDomLoc = chpl_rankChangeConvertDom(inds, inds.size, collapsedDim, idx);
      downDomLoc._value._free_when_no_arrs = true;
      downDomPid = downDomLoc._pid;
      downDomInst = downDomLoc._instance;
    }

    proc dsiAssignDomain(rhs: domain, lhsPrivate: bool) {
      chpl_assignDomainWithGetSetIndices(this, rhs);
    }

    iter these() {
      if chpl__isDROrDRView(downDom) {
        for i in upDom do
          yield i;
      } else {
        for i in downDom do
          yield downIdxToUpIdx(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone
      && !localeModelHasSublocales
      && chpl__isDROrDRView(downDom)
      && __primitive("method call resolves", upDom, "these", tag)
    {
      forall i in upDom do
        yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone
      && !localeModelHasSublocales
      && !chpl__isDROrDRView(downDom)
      && __primitive("method call resolves", downDom, "these", tag)
    {
      forall i in downDom do
        yield downIdxToUpIdx(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if chpl__isDROrDRView(downDom) {
        for followThis in upDom.these(tag) do
          yield followThis;
      } else {
        for followThis in downDom.these(tag) {
          const followThisLoD = chpl_rankChangeConvertHiDTupleToLoD(followThis);
          yield followThisLoD;
        }
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      if chpl__isDROrDRView(downDom) {
        for i in upDom.these(tag, followThis) do
          yield i;
      } else {
        const followThisHiD = chpl_rankChangeConvertLoDTupleToHiD(followThis);
        for i in downDom.these(tag, followThisHiD) {
          yield chpl_rankChangeConvertIdxHiDToLoD(i, collapsedDim, idx, rank);
        }
      }
    }

    proc chpl_rankChangeConvertLoDTupleToHiD(tup) {
      var tupHiD: downrank*tup(0).type;
      var i = 0;
      for param d in 0..downrank-1 do
        if collapsedDim(d) then
          tupHiD(d) = 0..0;
        else {
          tupHiD(d) = tup(i);
          i += 1;
        }
      return tupHiD;
    }

    proc chpl_rankChangeConvertHiDTupleToLoD(tup) {
      var tupLoD: rank*tup(0).type;
      var i = 0;
      for param d in 0..downrank-1 do
        if !collapsedDim(d) {
          tupLoD(i) = tup(d);
          i += 1;
        }
      return tupLoD;
    }


    inline proc downIdxToUpIdx(downIdx) {
      var upIdx: rank*idxType;
      var upDim = 0;
      for param downDim in 0..downrank-1 {
        if !collapsedDim(downDim) {
          upIdx(upDim) = downIdx(downDim);
          upDim += 1;
        }
      }
      if rank == 1 then
        return upIdx(0);
      else
        return upIdx;
    }

    // TODO: Is there something we can re-use here?
    proc dsiSerialWrite(f) throws {
      var first = true;
      for d in 0..downrank-1 do
        if !collapsedDim(d) {
          if first {
            f <~> "{";
            first = false;
          } else
            f <~> ", ";
          f <~> downDom.dsiDim(d);
        }
      f <~> "}";
    }

    override proc dsiMyDist() {
      return dist;
    }

    proc dsiTargetLocales() {
      //
      // BLC: there's a bit of a question in my mind about whether
      // rank-change slices (and regular slices for that matter) ought
      // to be expected to list only the subset of locales that have
      // non-empty subdomains for a given domain/array, or whether all
      // locales in the domain's/distribution's target locale set
      // should be listed since they are part of the target locale
      // set.  Both seem like they could be useful, though the former
      // seems as though it could be challenging to compute precisely
      // for less regular distributions.  Here I've done the easy
      // thing and simply returned all the locales that own the domain
      // below us, as slicing does.
      //
      return downDom.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return downDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain(loc: locale) {
      const dims = downDom.dsiLocalSubdomain(loc).dims();
      const empty : domain(rank, idxType, chpl__anyStridable(dims));

      // If the rank-changed dimension's index is not a member of the range
      // in the same dimension of 'dims', then this locale does not have a
      // local subdomain.
      for param d in 0..dims.size-1 {
        if collapsedDim(d) && dims(d).isEmpty() then
          return empty;
      }

      return chpl_rankChangeConvertDownToUp(dims, rank, collapsedDim);
    }

    override proc isRankChangeDomainView() param {
      return true;
    }

    proc _getActualDomain() {
      if chpl__isDomainView(downDom) {
        return downDom._getActualDomain();
      } else {
        return downDom;
      }
    }

    override proc dsiDestroyDom() {
      if upDomInst != nil then
        _delete_dom(upDomInst!, false);
      if downDomInst != nil then
        _delete_dom(downDomInst!, _isPrivatized(downDomInst!));
    }

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    override proc dsiSupportsPrivatization() param
      return downDomInst!.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (upDomInst, collapsedDim, idx, distPid, distInst, downDomPid, downDomInst);
    }

    proc dsiPrivatize(privatizeData) {
      return new unmanaged ArrayViewRankChangeDom(rank = this.rank,
                                        idxType = this.idxType,
                                        stridable = this.stridable,
                                        upDomInst = privatizeData(0),
                                        collapsedDim = privatizeData(1),
                                        idx = privatizeData(2),
                                        distPid = privatizeData(3),
                                        distInst = privatizeData(4),
                                        downDomPid = privatizeData(5),
                                        downDomInst = privatizeData(6));
    }

    proc dsiGetReprivatizeData() {
      return (upDomInst, downDomPid, downDomInst);
    }

    proc dsiReprivatize(other, reprivatizeData) {
      upDomInst = reprivatizeData(0);
      //      collapsedDim = other.collapsedDim;
      //      idx = other.idx;
      //      distPid = other.distPid;
      //      distInst = other.distInst;
      downDomPid = reprivatizeData(1);
      downDomInst = reprivatizeData(2);
    }

 } // end of class ArrayViewRankChangeDom

  private proc buildIndexCacheHelper(arr, dom, collapsedDim, idx) {
    if chpl__isDROrDRView(arr) {
      if (chpl__isArrayView(arr)) {
        if arr.isSliceArrayView() && !arr._containsRCRE() {
          // Only slices below in the view stack, which won't have built up
          // an indexCache.
          return arr._getActualArray().dsiGetRAD().toSlice(arr.dom).toRankChange(dom, collapsedDim, idx);
        } else {
          return arr.indexCache.toRankChange(dom, collapsedDim, idx);
        }
      } else {
        return arr.dsiGetRAD().toRankChange(dom, collapsedDim, idx);
      }
    } else {
      return false;
    }
  }

  //
  // The class representing a rank-change slice of an array.  Like
  // other array class implementations, it supports the standard dsi
  // interface.
  //
  pragma "aliasing array"
  class ArrayViewRankChangeArr: AbsBaseArr {
    // the representation of the slicing domain.  For a rank change
    // like A[lo..hi, 3] this is the lower-dimensional domain {lo..hi}.
    // It is represented as an ArrayViewRankChangeDom.
    //
    const _DomPid;
    const dom; // Seems like the compiler requires a field called 'dom'...

    // the higher-dimensional representation of the array being sliced
    const _ArrPid;
    const _ArrInstance;

    // These two fields represent whether or not each dimension was
    // collapsed as part of the rank-change; and if so, what the
    // index of that collapsed dimension was.  So for A[lo..hi, 3]
    // these would be (false, true) and (?, 3) respectively.

    // TODO: Use copies in domain rather than replicating these here?

    const collapsedDim;  // rank*bool    TODO: constrain this
    const idx;           // rank*idxType TODO: and this

    // If this is an array view on a DefaultRectangular array
    // (eventually...), the indexCache provides a mean of directly
    // accessing the array's ddata to avoid indirection overheads
    // through the array field above.
    const indexCache;

    const ownsArrInstance;

    proc init(type eltType, const _DomPid, const dom,
              const _ArrPid, const _ArrInstance,
              const collapsedDim, const idx,
              const ownsArrInstance : bool = false) {
      super.init(eltType = eltType);
      this._DomPid         = _DomPid;
      this.dom             = dom;
      this._ArrPid         = _ArrPid;
      this._ArrInstance    = _ArrInstance;
      this.collapsedDim    = collapsedDim;
      this.idx             = idx;
      this.indexCache      = buildIndexCacheHelper(_ArrInstance, dom, collapsedDim, idx);
      this.ownsArrInstance = ownsArrInstance;
    }

    // Forward all unhandled methods to underlying privatized array
    forwarding arr except these,
                      doiBulkTransferFromKnown, doiBulkTransferToKnown,
                      doiBulkTransferFromAny,  doiBulkTransferToAny, doiScan,
                      chpl__serialize, chpl__deserialize;



    //
    // standard generic aspects of arrays
    //

    // these could be fields, but indirecting works just as well and
    // makes the class less generic.
    proc idxType type return dom.idxType;
    proc rank param return dom.rank;

    // The following seems like it ought to work, but it causes an
    // error in the compiler for non-devel mode...  presumably due to
    // a direct query of eltType in the compiler(?).  As a TODO we
    // might want to hunt this down in the future...
    //
    //  proc eltType type return arr.eltType;


    //
    // introspection routine used elsewhere to filter array views
    //
    // TODO: Could this be replaced with more type-based introspection?
    // I shied away from it since this is a generic class, but there
    // must be (or should be) some way to do it without relying on
    // methods like this...
    //
    override proc isRankChangeArrayView() param {
      return true;
    }


    //
    // Helper routines to convert incoming low-D indices/domains into
    // their higher-D equivalents.
    //




    //
    // standard iterators
    //

    iter these() ref {
      for elem in chpl__serialViewIter(this, privDom) do
        yield elem;
    }

    // TODO: We seem to run into compile-time bugs when using multiple yields.
    // For now, work around them by using an if-expr
    iter these(param tag: iterKind) ref
      where tag == iterKind.standalone && !localeModelHasSublocales &&
           __primitive("method call resolves", privDom, "these", tag) {
      forall i in privDom {
        yield if shouldUseIndexCache()
                then indexCache.getDataElem(indexCache.getDataIndex(i))
                else arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in privDom.these(tag) do {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis) ref
      where tag == iterKind.follower {
      for i in privDom.these(tag, followThis) {
        if shouldUseIndexCache() {
          const dataIdx = indexCache.getDataIndex(i);
          yield indexCache.getDataElem(dataIdx);
        } else {
          yield arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
        }
      }
    }


    //
    // I/O
    //

    proc dsiSerialWrite(f) throws {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    proc dsiSerialRead(f) throws {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    override proc dsiDisplayRepresentation() {
      writeln("Rank Change view");
      writeln("----------");
      writeln("of domain:");
      privDom.dsiDisplayRepresentation();
      writeln("on array:");
      arr.dsiDisplayRepresentation();
      writeln("where the dims are collapsed as follows:");
      writeln(collapsedDim);
      writeln("and the missing indices are:");
      writeln(idx);
      writeln("----------");
    }

    //
    // accessors
    //

    inline proc dsiAccess(i: idxType ...rank) ref {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank)
      where shouldReturnRvalueByValue(eltType) {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i) ref {
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiLocalAccess(i) ref
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc dsiLocalAccess(i)
      where shouldReturnRvalueByValue(eltType)
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc dsiLocalAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc dsiBoundsCheck(i) {
      return privDom.dsiMember(i);
    }



    //
    // locality-oriented queries
    //

    proc dsiHasSingleLocalSubdomain() param
      return privDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain(loc: locale) {
      return privDom.dsiLocalSubdomain(loc);
    }

    //
    // privatization
    //

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    override proc dsiSupportsPrivatization() param
      return _ArrInstance.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (_DomPid, dom, _ArrPid, _ArrInstance, collapsedDim, idx);
    }

    proc dsiPrivatize(privatizeData) {
      return new unmanaged ArrayViewRankChangeArr(eltType=this.eltType,
                                        _DomPid=privatizeData(0),
                                        dom=privatizeData(1),
                                        _ArrPid=privatizeData(2),
                                        _ArrInstance=privatizeData(3),
                                        collapsedDim=privatizeData(4),
                                        idx=privatizeData(5));
    }

    //
    // utility functions used to set up the index cache
    //

    proc shouldUseIndexCache() param {
      return chpl__isDROrDRView(_ArrInstance);
    }

    proc buildIndexCache() {
      if shouldUseIndexCache() {
        if (chpl__isArrayView(_ArrInstance)) {
          if _ArrInstance.isSliceArrayView() && !_ArrInstance._containsRCRE() {
            // Only slices below in the view stack, which won't have built up
            // an indexCache.
            return _ArrInstance._getActualArray().dsiGetRAD().toSlice(_ArrInstance.dom).toRankChange(dom, collapsedDim, idx);
          } else {
            return _ArrInstance.indexCache.toRankChange(dom, collapsedDim, idx);
          }
        } else {
          return _ArrInstance.dsiGetRAD().toRankChange(dom, collapsedDim, idx);
        }
      } else {
        return false;
      }
    }


    //
    // routines relating to the underlying domains and arrays
    //

    inline proc privDom {
      if _isPrivatized(dom) {
        return chpl_getPrivatizedCopy(dom.type, _DomPid);
      } else {
        return dom;
      }
    }

    inline proc arr {
      if _isPrivatized(_ArrInstance) {
        return chpl_getPrivatizedCopy(_ArrInstance.type, _ArrPid);
      } else {
        return _ArrInstance;
      }
    }

    // not sure what this is, but everyone seems to have one...
    override proc dsiGetBaseDom() {
      return dom;
    }

    proc _getActualArray() {
      if chpl__isArrayView(arr) {
        return arr._getActualArray();
      } else {
        return arr;
      }
    }

    proc _containsRCRE() param {
      if chpl__isArrayView(arr) {
        return (arr.isRankChangeArrayView() ||
                arr.isReindexArrayView() ||
                arr._containsRCRE());
      } else {
        return false;
      }
    }

    proc _getRCREView() {
      return this;
    }

    override proc dsiElementInitializationComplete() {
      // no elements allocated here, so no action necessary
    }

    override proc dsiDestroyArr(deinitElts:bool) {
      if ownsArrInstance {
        _delete_arr(_ArrInstance, _isPrivatized(_ArrInstance));
      }
    }

    //
    // A RankChange will only attempt a bulk-transfer if its underlying array
    // has explicitly opted-in by implementing the param method
    // ``doiCanBulkTransferRankChange`` that returns true. This will help
    // domain-map authors avoid this tricky case without any effort on their
    // part.
    //

    override proc doiCanBulkTransferRankChange() param
      return arr.doiCanBulkTransferRankChange();

    proc doiBulkTransferFromKnown(destDom, srcClass, srcDom) : bool
    where this.arr.doiCanBulkTransferRankChange() {
      const shifted = chpl_rankChangeConvertDom(destDom.dims(), destDom.rank, this.dom.collapsedDim, this.dom.idx);
      return chpl__bulkTransferArray(this.arr, shifted, srcClass, srcDom);
    }

    proc doiBulkTransferToKnown(srcDom, destClass, destDom) : bool
    where this.arr.doiCanBulkTransferRankChange() {
      const shifted = chpl_rankChangeConvertDom(srcDom.dims(), srcDom.rank, this.dom.collapsedDim, this.dom.idx);
      return chpl__bulkTransferArray(destClass, destDom, this.arr, shifted);
    }
  }  // end of class ArrayViewRankChangeArr


  // helper routines for converting things from higher- to
  // lower-dimensions and back again

  inline proc chpl_rankChangeConvertIdx(i: integral, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    var ind = idx;
    var j = 0;
    for param d in 0..downrank-1 {
      if !collapsedDim(d) {
        ind(d) = i;
        j += 1;
      }
    }
    assert (j == 1);
    return ind;
  }

  inline proc chpl_rankChangeConvertIdx(i, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    var ind = idx;
    var j = 0;
    for param d in 0..downrank-1 {
      if !collapsedDim(d) {
        ind(d) = i(j);
        j += 1;
      }
    }
    return ind;
  }

  inline proc chpl_rankChangeConvertIdxHiDToLoD(i, collapsedDim, idx, param rank) {
    param downrank = collapsedDim.size;
    var ind: rank*i(0).type;
    var j = 0;
    for param d in 0..downrank-1 {
      if !collapsedDim(d) {
        ind(j) = i(d);
        j += 1;
      }
    }
    if rank == 1 then
      return ind(0);
    else
      return ind;
  }

  inline proc chpl_rankChangeConvertDownToUp(dims, param uprank, collapsedDim) {
    var ranges : uprank*dims(0).type;
    var j = 0;
    for param d in 0..dims.size-1 {
      if !collapsedDim(d) {
        ranges(j) = dims(d);
        j += 1;
      }
    }
    return {(...ranges)};
  }

  inline proc chpl_rankChangeConvertDom(dims, param uprank, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    if uprank != dims.size then
      compilerError("Called chpl_rankChangeConvertDom with incorrect rank. Got ", dims.size:string, ", expecting ", uprank:string);

    var ranges : downrank*dims(0).type;
    var j = 0;
    for param d in 0..downrank-1 {
      if !collapsedDim(d) {
        ranges(d) = dims(j);
        j += 1;
      } else {
        ranges(d) = idx(d)..idx(d);
      }
    }
    return {(...ranges)};
  }
}
