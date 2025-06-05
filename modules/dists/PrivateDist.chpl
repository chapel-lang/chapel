/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

/* A distribution for mapping domain/array indices to locales in a 1:1 manner. */

@unstable("PrivateDist is unstable and may change in the future")
prototype module PrivateDist {

use ChplConfig only compiledForSingleLocale;
use DSIUtil;

//
// Private Distribution, Domain, and Array
//  Defines PrivateSpace, an instance of PrivateDom
//
/*
This ``privateDist`` distribution maps each index ``i``
between ``0`` and ``numLocales-1`` to ``Locales[i]``.

The index set of a domain distributed over ``privateDist``
is always ``0..numLocales-1``, regardless of the domain's rank,
and cannot be changed.

The following domain is available as a convenience,
so user programs do not need to declare their own:

  .. code-block:: chapel

    const PrivateSpace: domain(1) dmapped new privateDist();


**Example**

The following code declares a Private-distributed array ``A``.
The `forall` loop visits each locale and sets the array element
corresponding to that locale to that locale's number of cores.

  .. code-block:: chapel

    var A: [PrivateSpace] int;
    forall a in A do
      a = here.numPUs();


**Data-Parallel Iteration**

A `forall` loop over a ``privateDist`` domain or array
runs a single task on each locale.
That task executes the loop's iteration corresponding to
that locale's index in the ``Locales`` array.


**Limitations**

Domains and arrays distributed over this distribution
do not provide some standard domain/array functionality.

This distribution may perform unnecessary communication
between locales.
*/

record privateDist: writeSerializable {
  forwarding const chpl_distHelp: chpl_PrivatizedDistHelper(unmanaged PrivateImpl);

  proc init() {
    const value = new unmanaged PrivateImpl();
    this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                          if _isPrivatized(value)
                            then _newPrivatizedClass(value)
                            else nullPid,
                          value);
  }

    proc init(_pid : int, _instance, _unowned : bool) {
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(_pid,
                                                         _instance,
                                                         _unowned);
    }

    proc init(value) {
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                             if _isPrivatized(value)
                               then _newPrivatizedClass(value)
                               else nullPid,
                             _to_unmanaged(value));
    }

    // does not match the type of 'other'. That case is handled by the compiler
    // via coercions.
    proc init=(const ref other : privateDist) {
      this.init(other._value.dsiClone());
    }

    proc clone() {
      return new privateDist(this._value.dsiClone());
    }

  @chpldoc.nodoc
  inline operator ==(d1: privateDist, d2: privateDist) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  @chpldoc.nodoc
  inline operator !=(d1: privateDist, d2: privateDist) {
    return !(d1 == d2);
  }

  proc serialize(writer, ref serializer) throws {
    chpl_distHelp.serialize(writer, serializer);
  }
}


@chpldoc.nodoc
@unstable(category="experimental", reason="assignment between distributions is currently unstable due to lack of testing")
operator =(ref a: privateDist, b: privateDist) {
  if a._value == nil {
    __primitive("move", a, chpl__autoCopy(b.clone(), definedConst=false));
  } else {
    if a._value.type != b._value.type then
      compilerError("type mismatch in distribution assignment");
    if a._value == b._value {
      // do nothing
    } else
        a._value.dsiAssign(b._value);
    if _isPrivatized(a._instance) then
      _reprivatize(a._value);
  }
}


@deprecated("'Private' is deprecated, please use 'privateDist' instead")
type Private = privateDist;


@chpldoc.nodoc
class PrivateImpl: BaseDist, writeSerializable {
  override proc dsiNewRectangularDom(param rank: int, type idxType,
                                     param strides: strideKind, inds) {
    for i in inds do
      if i.size != 0 then
        halt("Tried to create a privateDom with a specific index set");

    return new unmanaged PrivateDom(rank=rank, idxType=idxType,
                                    strides=strides,
                                    dist=_to_unmanaged(this));
  }

  override proc serialize(writer, ref serializer) throws {
    writer.writeln("Private Distribution");
  }

  // acts like a singleton
  proc dsiClone() do return _to_unmanaged(this);

  proc trackDomains() param do return false;

  override proc dsiTrackDomains() do    return false;

  override proc singleton() param do return true;
}

class PrivateDom: BaseRectangularDom(?) {
  var dist: unmanaged PrivateImpl;

  iter these() { for i in 0..numLocales-1 do yield i; }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    coforall loc in Locales do on loc {
      var t: 1*range(idxType);
      t(0) = here.id..here.id;
      yield t;
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis(0) do
      yield i;
  }

  proc dsiSerialWrite(x) { x.write("Private Domain"); }

  proc dsiBuildArray(type eltType, param initElts:bool) {
    return new unmanaged PrivateArr(eltType=eltType, rank=rank,
                                    idxType=idxType,
                                    strides=strides,
                                    dom=_to_unmanaged(this),
                                    initElts=initElts);
  }

  proc dsiNumIndices do return numLocales;
  override proc dsiLow do return 0;
  override proc dsiAlignedLow do return 0;
  override proc dsiHigh do return numLocales-1;
  override proc dsiAlignedHigh do return numLocales-1;
  override proc dsiStride do return 0;
  proc dsiSetIndices(x: domain) { halt("cannot reassign private domain"); }
  proc dsiGetIndices() { return {0..numLocales-1}; }

  proc dsiDim(d : int) {
    return dsiLow..dsiHigh;
  }

  proc dsiDims() {
    return (dsiLow..dsiHigh,);
  }

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    halt("cannot reassign private domain");
  }

  override proc dsiRequiresPrivatization() param do return true;
  override proc linksDistribution() param do return false;

  override proc dsiLinksDistribution() do return false;

  proc dsiGetPrivatizeData() do return 0;

  proc dsiPrivatize(privatizeData) {
    return new unmanaged PrivateDom(rank=rank, idxType=idxType,
                                    strides=strides,
                                    dist=dist);
  }

  proc dsiGetReprivatizeData() do return 0;

  proc dsiReprivatize(other, reprivatizeData) { }

  proc dsiMember(i) do return (0 <= i && i <= numLocales-1);

  override proc dsiMyDist() do return dist;

  proc dsiGetDist() {
    if _isPrivatized(dist) then
      return new privateDist(dist.pid, dist, _unowned=true);
    else
      return new privateDist(nullPid, dist, _unowned=true);
}
}

private proc checkCanMakeDefaultValue(type eltType) param {
  var default: eltType;
}

class PrivateArr: BaseRectangularArr(?) {

  var dom: unmanaged PrivateDom(rank, idxType, strides);

  // may be initialized separately
  // always destroyed explicitly (to control deiniting elts)
  pragma "no init" pragma "unsafe" pragma "no auto destroy"
  var data: eltType;

  var isPrivatizedCopy: bool;
  var defaultInitDataOnPrivatize: bool;

  proc init(type eltType,
            param rank,
            type idxType,
            param strides,
            dom: unmanaged PrivateDom(rank, idxType, strides),
            param initElts: bool) {
    super.init(eltType=eltType, rank=rank, idxType=idxType,
               strides=strides);
    this.dom = dom;
    // this.data not initialized
    this.isPrivatizedCopy = false;
    this.defaultInitDataOnPrivatize = initElts;

    if initElts {
      pragma "no auto destroy"
      var default: eltType;
      __primitive("=", this.data, default);
    }
  }

  proc init(toPrivatize: PrivateArr) {
    var privdom = chpl_getPrivatizedCopy(toPrivatize.dom.type,
                                         toPrivatize.dom.pid);
    super.init(eltType=toPrivatize.eltType, rank=toPrivatize.rank,
               idxType=toPrivatize.idxType, strides=toPrivatize.strides);
    this.dom = privdom;
    // this.data not initialized
    this.isPrivatizedCopy = true;
    this.defaultInitDataOnPrivatize = toPrivatize.defaultInitDataOnPrivatize;
    init this;

    if defaultInitDataOnPrivatize {
      pragma "no auto destroy" pragma "unsafe"
      var default: eltType;
      __primitive("=", this.data, default);
    }
  }

  proc deinit() {
    // data is deinited in dsiDestroyArr if necessary.

  }

  override proc dsiIteratorYieldsLocalElements() param {
    return true;
  }
}

override proc PrivateArr.dsiElementInitializationComplete() {
  // no action necessary
}

override proc PrivateArr.dsiElementDeinitializationComplete() {
  // no action necessary
}

override proc PrivateArr.dsiDestroyArr(deinitElts:bool) {
  if deinitElts {
    param needsDestroy = __primitive("needs auto destroy", eltType);

    if needsDestroy {
      if compiledForSingleLocale() {
        chpl__autoDestroy(data);
      } else {
        const pid = this.pid;
        coforall loc in Locales {
          on loc {
            var privarr = chpl_getPrivatizedCopy(_to_unmanaged(this.type), pid);
            chpl__autoDestroy(privarr.data);
          }
        }
      }
    }
  }
}

override proc PrivateArr.dsiGetBaseDom() do return dom;

override proc PrivateArr.dsiRequiresPrivatization() param do return true;

proc PrivateArr.dsiGetPrivatizeData() do return 0;

proc PrivateArr.dsiPrivatize(privatizeData) {
  return new unmanaged PrivateArr(toPrivatize=this);
}

proc PrivateArr.dsiAccess(i: idxType) ref {
  if compiledForSingleLocale() then
    return data;
  else if i == here.id then
    return data;
  else {
    var privarr = _to_unmanaged(this);
    on Locales(i) {
      privarr = chpl_getPrivatizedCopy(_to_unmanaged(this.type), this.pid);
    }
    return privarr.data;
  }
}

proc PrivateArr.dsiAccess(i: 1*idxType) ref do
  return dsiAccess(i(0));

proc PrivateArr.dsiBoundsCheck(i: 1*idxType) {
  var idx = i(0);
  return 0 <= idx && idx < numLocales;
}

iter PrivateArr.these() ref {
  foreach i in dom do
    yield dsiAccess(i);
}

iter PrivateArr.these(param tag: iterKind) where tag == iterKind.leader {
  coforall loc in Locales do on loc {
    var t: 1*range(idxType);
    t(0) = here.id..here.id;
    yield t;
  }
}

iter PrivateArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  foreach i in followThis(0) do
    yield dsiAccess(i);
}

proc PrivateArr.dsiSerialWrite(x) {
  var first: bool = true;
  for i in dom {
    if first then first = !first; else x.write(" ");
    x.write(dsiAccess(i));
  }
}

proc PrivateArr.doiScan(op, dom) where (rank == 1) &&
                                  chpl__scanStateResTypesMatch(op) {
  type resType = op.generate().type;
  var res: [dom] resType;

  var localArr: [0..numLocales-1] resType;

  coforall loc in Locales with (ref localArr) do on loc do
    localArr[here.id] = if _isPrivatized(this) then chpl_getPrivatizedCopy(this.type, this.pid).data else data;

  var localRes = localArr._scan(op);

  forall r in res do r = localRes[here.id];

  // localArr deletes op
  return res;
}

// TODO: Fix 'new Private()' leak -- Discussed in #6726
// ENGIN: below is my workaround to close the leak:
// 1. Declare a module-scope record variable with an unmanaged nilable Private
//    field
//    1.a. Make sure that this variable is defined *before* PrivateSpace, so
//         that compiler injects its cleanup *after* PrivateSpace
// 2. In module deinitializer, set the field of this record variable to
//    chpl_privateDist
// This way we cleanup the unmanaged, module-scope variable after module
// deinitializer, which is called before deinitializing module-scope variables
// which would cause use-after-free during the cleanup of PrivateSpace
var chpl_privateCW = new chpl_privateDistCleanupWrapper();
var chpl_privateDist = new unmanaged PrivateImpl();
const PrivateSpace: domain(1) dmapped new privateDist(chpl_privateDist);

record chpl_privateDistCleanupWrapper {
  var val = nil : unmanaged PrivateImpl?;
  proc deinit() { delete val!; }
}

proc deinit() {
  chpl_privateCW.val = chpl_privateDist;
}

} // PrivateDist
