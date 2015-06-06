/*
 * Copyright 2004-2015 Cray Inc.
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
  
  class DefaultOpaqueDom: BaseOpaqueDom {
    type idxType = _OpaqueIndex;
    param parSafe: bool;
    var dist: DefaultDist;
    var adomain: DefaultAssociativeDom(idxType=_OpaqueIndex, parSafe=parSafe);
  
    proc linksDistribution() param return false;
    proc dsiLinksDistribution()     return false;
  
    proc DefaultOpaqueDom(dist: DefaultDist, param parSafe: bool) {
      this.dist = dist;
      adomain = new DefaultAssociativeDom(_OpaqueIndex, dist, parSafe=parSafe);
      // This prevents premature deletion of the contained associated dom, in
      // case it is iterated upon, or other operations that might cause the
      // refCount to take an excursion from zero and return there.
      // When the ref count of a dom goes to zero the dom is reclaimed (freed),
      // and that would be bad.
      adomain.incRefCount();
    }
  
    proc ~DefaultOpaqueDom() {
      // We assume that adomain is not shared out, so that when the containing
      // opaque dom is deleted, the contained associative domain can be free
      // unconditionally.
      for i in adomain do delete i;
      delete adomain;
    }
  
    proc dsiCreate() {
      var i = new _OpaqueIndex();
      adomain.dsiAdd(i);
      return i;
    }
  
    proc dsiGetIndices() return adomain;
  
    proc dsiSetIndices(b: DefaultAssociativeDom) {
      adomain.dsiSetIndices(b);
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
  
    proc dsiMember(ind: idxType) {
      return adomain.dsiMember(ind);
    }
  
    proc dsiNumIndices {
      return adomain.dsiNumIndices;
    }
  
    proc dsiBuildArray(type eltType) {
      var ia = new DefaultOpaqueArr(eltType=eltType, idxType=idxType, parSafe=parSafe, dom=this);
      return ia;
    }
  }
  
  pragma "auto copy fn"
  proc chpl__autoCopy(x: DefaultOpaqueDom) {
    if ! noRefCount then
      x.incRefCount();
    return x;
  }
  
  proc chpl__autoDestroy(x: DefaultOpaqueDom) {
    if !noRefCount {
      var cnt = x.destroyDom();
      if cnt == 0 then
        delete x;
    }
  }

  proc DefaultOpaqueDom.dsiRemove(idx: idxType) {
    adomain.dsiRemove(idx);
  }
  
  proc DefaultOpaqueDom.dsiSerialWrite(f: Writer) {
    adomain.dsiSerialWrite(f);
  }
  
  proc DefaultOpaqueDom.dsiSerialRead(f: Reader) {
    adomain.dsiSerialRead(f);
  }
  
  proc DefaultOpaqueArr.dsiSerialWrite(f: Writer) {
    anarray.dsiSerialWrite(f);
  }
  proc DefaultOpaqueArr.dsiSerialRead(f: Reader) {
    anarray.dsiSerialRead(f);
  }
  
  
  class DefaultOpaqueArr: BaseArr {
    type eltType;
    type idxType;
    param parSafe: bool;
  
    var dom: DefaultOpaqueDom(idxType=idxType, parSafe=parSafe);
    var anarray = new DefaultAssociativeArr(eltType=eltType, idxType=idxType,
                                            parSafeDom=parSafe, dom=dom.adomain);
  
    proc initialize() {
      // In spite of what was promised, the domain owned by the above
      // DefaultOpaqueDom is shared out in the call to new
      // DefaultAssociativeArr() above, so we have to bump its ref count here.
      dom.adomain.incRefCount();
      // We have to bump the reference count to move it away from zero.
      anarray.incRefCount();
    }

    proc ~DefaultOpaqueArr() {
      // Something more may be needed here.
      delete anarray; anarray = nil;

      on dom {
        local dom.remove_arr(this);
        if ! noRefCount {
          var cnt = dom.destroyDom();
          if cnt == 0 then
            delete dom;
        }
      }
    }
  
    proc dsiGetBaseDom() return dom;
  
    proc dsiAccess(ind : idxType) ref : eltType
      return anarray.dsiAccess(ind);

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by opaque domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      return _newDomain(dom);
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
  
  
    iter dsiSorted() {
      for e in anarray.dsiSorted() do
        yield e;
    }
  }
  
  pragma "auto copy fn"
  proc chpl__autoCopy(x: DefaultOpaqueArr) {
    if !noRefCount then
      x.incRefCount();
    return x;
  }
  
  proc chpl__autoDestroy(x: DefaultOpaqueArr) {
    if !noRefCount {
      var cnt = x.destroyArr();
      if cnt == 0 then
        delete x;
    }
  }

}
