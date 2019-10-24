/*
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

// DefaultOpaque.chpl
//
module DefaultOpaque {
  private use ChapelStandard;

  // record _OpaqueIndex is defined in ChapelArray

  pragma "no doc"
  proc _OpaqueIndexGetNext():uint {
    var n:uint;
    local {
      n = _OpaqueIndexNext.fetchAdd(1, order=memoryOrder.relaxed);
    }
    return n+1;
  }

  // creating a new opaque index
  proc _OpaqueIndexCreate() {
    var idx:_OpaqueIndex;
    idx.node = here.id;
    idx.i = _OpaqueIndexGetNext();
    return idx;
  }
  // support for pretending the default value is nil
  inline proc =(ref a:_OpaqueIndex, b:_nilType) {
    a.i = 0;
  }
  inline proc ==(a:_OpaqueIndex, b:_nilType) {
    return a.i == 0;
  }
  inline proc ==(a:_nilType, b:_OpaqueIndex) {
    return b.i == 0;
  }
  inline proc !=(a:_OpaqueIndex, b:_nilType) {
    return a.i != 0;
  }
  inline proc !=(a:_nilType, b:_OpaqueIndex) {
    return b.i != 0;
  }
  proc _OpaqueIndex.writeThis(writer) {
    writer <~> "{}";
  }

  class DefaultOpaqueDom: BaseOpaqueDom {
    type idxType = _OpaqueIndex;
    param parSafe: bool;
    var dist: unmanaged DefaultDist;
    var adomain: unmanaged DefaultAssociativeDom(idxType=_OpaqueIndex, parSafe=parSafe);
  
    proc linksDistribution() param return false;
    override proc dsiLinksDistribution() return false;
  
    proc init(dist: unmanaged DefaultDist, param parSafe: bool) {
      this.parSafe = parSafe;
      this.dist = dist;
      adomain = new unmanaged DefaultAssociativeDom(_OpaqueIndex, dist, parSafe=parSafe);
    }
  
    proc deinit() {
      delete adomain;
    }
  
    proc dsiCreate() {
      var i = _OpaqueIndexCreate();
      adomain.dsiAdd(i);
      return i;
    }

    override proc dsiMyDist() {
      return dist;
    }

    proc dsiAdd(i:idxType) {
      return adomain.dsiAdd(i);
    }
  
    proc dsiGetIndices() return adomain;
  
    proc dsiSetIndices(b: unmanaged DefaultAssociativeDom) {
      adomain.dsiSetIndices(b);
    }
  
    proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
      chpl_assignDomainWithIndsIterSafeForRemoving(this, rhs);
    }

    iter these() {
      for i in adomain do
        yield i;
    }
  
    iter these(param tag: iterKind) where tag == iterKind.leader {
      for block in adomain.these(tag=iterKind.leader) do
        yield block;
    }
  
    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      for i in adomain.these(tag=iterKind.follower, followThis) do
        yield i;
    }

    iter dsiIndsIterSafeForRemoving() {
      for i in adomain.dsiIndsIterSafeForRemoving() {
        yield i;
      }
    }

    proc dsiMember(ind: idxType) {
      return adomain.dsiMember(ind);
    }
  
    proc dsiNumIndices {
      return adomain.dsiNumIndices;
    }
  
    proc dsiBuildArray(type eltType) {
      var ia = new unmanaged DefaultOpaqueArr(eltType=eltType, idxType=idxType, parSafe=parSafe, dom=_to_unmanaged(this));
      return ia;
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(_to_unmanaged(this));
      } else {
        var a: domain(opaque);
        return a;
      }
    }
  }
  
  proc DefaultOpaqueDom.dsiSerialWrite(f) {
    adomain.dsiSerialWrite(f);
  }
  
  proc DefaultOpaqueDom.dsiSerialRead(f) {
    adomain.dsiSerialRead(f);
  }
  
  proc DefaultOpaqueArr.dsiSerialWrite(f) {
    anarray.dsiSerialWrite(f);
  }
  proc DefaultOpaqueArr.dsiSerialRead(f) {
    anarray.dsiSerialRead(f);
  }
  
  
  class DefaultOpaqueArr: AbsBaseArr {
    type idxType;
    param parSafe: bool;
  
    var dom: unmanaged DefaultOpaqueDom(idxType=idxType, parSafe=parSafe);
    var anarray = new unmanaged DefaultAssociativeArr(eltType=eltType, idxType=idxType, parSafeDom=parSafe, dom=dom.adomain);
  
    proc deinit() {
      delete anarray;
    }
  
    override proc dsiGetBaseDom() return dom;
  
    proc dsiAccess(ind : idxType) ref : eltType
      return anarray.dsiAccess(ind);

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by opaque domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(dom);
      } else {
        var a: domain(opaque);
        return a;
      }
    }
  
    iter these() ref {
      for e in anarray do
        yield e;
    }
  
    iter these(param tag: iterKind) where tag == iterKind.leader {
      for block in dom.these(tag=iterKind.leader) do
        yield block;
    }
  
    iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
      for i in dom.these(tag=iterKind.follower, followThis) do
        yield dsiAccess(i);
    }
  
  
    iter dsiSorted(comparator) {
      for e in anarray.dsiSorted(comparator) do
        yield e;
    }
  }
  
  proc DefaultOpaqueDom.dsiRemove(idx: idxType) {
    return adomain.dsiRemove(idx);
  }
  
}
