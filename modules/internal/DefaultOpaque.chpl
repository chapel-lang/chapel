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
    }
  
    proc ~DefaultOpaqueDom() {
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
  
    proc ~DefaultOpaqueArr() {
      delete anarray;
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
  
  proc DefaultOpaqueDom.dsiRemove(idx: idxType) {
    adomain.dsiRemove(idx);
  }
  
}
