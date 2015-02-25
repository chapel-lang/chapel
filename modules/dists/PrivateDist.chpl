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

//
// Private Distribution, Domain, and Array
//  Defines PrivateSpace, an instance of PrivateDom
//
class Private: BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable);
  }

  proc writeThis(x: Writer) {
    x.writeln("Private Distribution");
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(x: Private) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: Private) {
  if !noRefCount {
    var cnt = x.destroyDist();
    if cnt == 0 then
      delete x;
  }
}

class PrivateDom: BaseRectangularDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var pid: int = -1;
  var dist: Private;

  iter these() { for i in 0..numLocales-1 do yield i; }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    coforall loc in Locales do on loc {
      var t: 1*range(idxType);
      t(1) = here.id..here.id;
      yield t;
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis(1) do
      yield i;
  }

  proc dsiSerialWrite(x: Writer) { x.write("Private Domain"); }

  proc dsiBuildArray(type eltType)
    return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);

  proc dsiNumIndices return numLocales;
  proc dsiLow return 0;
  proc dsiHigh return numLocales-1;
  proc dsiStride return 0;
  proc dsiSetIndices(x: domain) { compilerError("cannot reassign private domain"); }
  proc dsiGetIndices() { return {0..numLocales-1}; }

  proc dsiRequiresPrivatization() param return true;
  proc linksDistribution() param return false;
  proc dsiLinksDistribution()     return false;

  proc dsiGetPrivatizeData() return 0;

  proc dsiPrivatize(privatizeData)
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  proc dsiGetReprivatizeData() return 0;

  proc dsiReprivatize(other, reprivatizeData) { }

  proc dsiMember(i) return 0 <= i && i <= numLocales-1;
}

pragma "auto copy fn"
proc chpl__autoCopy(x: PrivateDom) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: PrivateDom) {
  if !noRefCount {
    var cnt = x.destroyDom();
    if cnt == 0 then
      delete x;
  }
}

class PrivateArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var dom: PrivateDom(rank, idxType, stridable);
  var data: eltType;
  var pid: int = -1;
}

pragma "auto copy fn"
proc chpl__autoCopy(x: PrivateArr) {
  if !noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: PrivateArr) {
  if !noRefCount {
    var cnt = x.destroyArr();
    if cnt == 0 then
      delete x;
  }
}

proc PrivateArr.dsiGetBaseDom() return dom;

proc PrivateArr.dsiRequiresPrivatization() param return true;

proc PrivateArr.dsiGetPrivatizeData() return 0;

proc PrivateArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
}

proc PrivateArr.dsiAccess(i: idxType) ref {
  if _local then
    return data;
  else if i == here.id then
    return data;
  else {
    if boundsChecking then
      if i < 0 || i >= numLocales then
        halt("array index out of bounds: ", i);
    var privarr = this;
    on Locales(i) {
      privarr = chpl_getPrivatizedCopy(this.type, this.pid);
    }
    return privarr.data;
  }
}

proc PrivateArr.dsiAccess(i: 1*idxType) ref
  return dsiAccess(i(1));

iter PrivateArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

iter PrivateArr.these(param tag: iterKind) where tag == iterKind.leader {
  coforall loc in Locales do on loc {
    var t: 1*range(idxType);
    t(1) = here.id..here.id;
    yield t;
  }
}

iter PrivateArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  for i in followThis(1) do
    yield dsiAccess(i);
}

proc PrivateArr.dsiSerialWrite(x: Writer) {
  var first: bool = true;
  for i in dom {
    if first then first = !first; else write(" ");
    write(dsiAccess(i));
  }
}

const PrivateSpace: domain(1) dmapped new dmap(new Private());
