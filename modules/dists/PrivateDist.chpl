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
// Private Distribution, Domain, and Array
//  Defines PrivateSpace, an instance of PrivateDom
//
/*
This Private distribution maps each index ``i``
between ``0`` and ``numLocales-1`` to ``Locales[i]``.

The index set of a domain distributed over a Private distribution
is always ``0..numLocales-1``, regardless of the domain's rank,
and cannot be changed.

The following domain is available as a convenience,
so user programs do not need to declare their own:

  .. code-block:: chapel

    const PrivateSpace: domain(1) dmapped Private();


**Example**

The following code declares a Private-distributed array ``A``.
The `forall` loop visits each locale and sets the array element
corresponding to that locale to that locale's number of cores.

  .. code-block:: chapel

    var A: [PrivateSpace] int;
    forall a in A do
      a = here.numPUs();


**Data-Parallel Iteration**

A `forall` loop over a Private-distributed domain or array
runs a single task on each locale.
That task executes the loop's iteration corresponding to
that locale's index in the ``Locales`` array.


**Limitations**

Domains and arrays distributed over this distribution
do not provide some standard domain/array functionality.

This distribution may perform unnecessary communication
between locales.
*/
class Private: BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
    for i in inds do
      if i.size != 0 then
        halt("Tried to create a privateDom with a specific index set");
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable, dist=this);
  }

  proc writeThis(x) {
    x <~> "Private Distribution\n";
  }
  // acts like a singleton
  proc dsiClone() return this;

  proc trackDomains() param return false;
  proc dsiTrackDomains()    return false;

  proc singleton() param return true;
}

class PrivateDom: BaseRectangularDom {
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

  proc dsiSerialWrite(x) { x <~> "Private Domain"; }

  proc dsiBuildArray(type eltType)
    return new PrivateArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);

  proc dsiNumIndices return numLocales;
  proc dsiLow return 0;
  proc dsiHigh return numLocales-1;
  proc dsiStride return 0;
  proc dsiSetIndices(x: domain) { halt("cannot reassign private domain"); }
  proc dsiGetIndices() { return {0..numLocales-1}; }

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    halt("cannot reassign private domain");
  }

  proc dsiRequiresPrivatization() param return true;
  proc linksDistribution() param return false;
  proc dsiLinksDistribution()     return false;

  proc dsiGetPrivatizeData() return 0;

  proc dsiPrivatize(privatizeData)
    return new PrivateDom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  proc dsiGetReprivatizeData() return 0;

  proc dsiReprivatize(other, reprivatizeData) { }

  proc dsiMember(i) return 0 <= i && i <= numLocales-1;
  proc dsiMyDist() return dist;
}

class PrivateArr: BaseRectangularArr {
  var dom: PrivateDom(rank, idxType, stridable);
  var data: eltType;
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

proc PrivateArr.dsiSerialWrite(x) {
  var first: bool = true;
  for i in dom {
    if first then first = !first; else x <~> " ";
    x <~> dsiAccess(i);
  }
}

// TODO: Fix 'new Private()' leak -- Discussed in #6726
const PrivateSpace: domain(1) dmapped new dmap(new Private());

