/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

/*
  The :mod:`Initialization` module provides functions which enable users to
  move-initialize and deinitialize values.

  The functionality provided by this module can be used to implement
  collections in a manner similar to those implemented by the Chapel standard
  modules (such as :mod:`List` or :mod:`Map`).
*/
module Initialization {

  private inline proc _move(ref dst: ?t, const ref src: t) {
    __primitive("=", dst, src);
  }

  /*
    Check to see if a given type supports deinitialization.

    :arg t: A type to check for a deinitializer
    :type t: `type`

    :return: ``true`` if ``t`` supports deinitialization
    :rtype: param bool
  */
  proc needsDeinit(type t) param {
    return __primitive("needs auto destroy", t);
  }

  /*
    Explicitly deinitialize a reference. The memory referred to by ``arg``
    should be considered uninitialized after a call to this function.

    .. warning::

      Chapel is not aware of a call to ``explicitDeinit()``, which means
      that the deinitializer of ``arg`` may run again once it goes out of
      scope.

    :arg: A value to deinitialize
  */
  proc explicitDeinit(ref arg: ?t) {
    if needsDeinit(t) then
      chpl__autoDestroy(arg);
  }

  /*
    Move-initialize a reference from a value. No assignment is performed
    when moving ``rhs`` into the memory referred to by ``lhs``.

    .. warning::

      If the value at ``lhs`` is already initialized memory, it will be
      overwritten by the contents of ``rhs`` without being deinitialized.
      Call :proc:`explicitDeinit()` to deinitialize ``lhs`` first if
      necessary.

    :arg lhs: A reference to move-initialize
    :type lhs: `?t`

    :arg rhs: A 
    :type rhs: `t`
  */
  proc moveInitialize(ref lhs: ?t, pragma "no auto destroy" in rhs: t) {
    if lhs.type != nothing then
      _move(lhs, rhs);
  }

  /*
    Move the memory referred to by a reference into a new value.

    .. warning::

      The memory referred to by ``arg`` should be considered uninitialized
      after a call to this function.

    :arg arg: A reference to consume
    :type arg: `?t`

    :return: A reference moved into a new value
    :rtype: `t`
  */
  proc moveToValue(const ref arg: ?t) {
    if t == nothing {
      return none;
    } else {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var result: t;
      _move(result, arg);
      return result;
    }
  }

  /*
    Swap the values referred to by two references. No assignment takes
    place when swapping ``lhs`` and ``rhs``.

    :arg lhs: A value to swap
    :type lhs: `?t`

    :arg rhs: A value to swap
    :type rhs: `t`
  */
  proc moveSwap(ref lhs: ?t, ref rhs: t) {
    if t == nothing {
      return none;
    } else {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var temp: t;
      _move(temp, lhs);
      _move(lhs, rhs);
      _move(rhs, temp);
    }
  }

  private inline proc _haltBadIndex(a, idx, indexName: string) {
    import HaltWrappers.boundsCheckHalt;

    if !a.domain.contains(idx) then
      boundsCheckHalt('Cannot move-initialize array because its domain ' +
                      'does not contain: ' + indexName);
  }

  // Call to 'indexOrder' assumes 'idx' is a valid index.
  private inline proc _haltBadElementRange(a, idx, numElements: int) {
    import HaltWrappers.boundsCheckHalt;

    if numElements > a.size then
      boundsCheckHalt('Cannot move-initialize array because number of ' +
                      'elements to copy exceeds array size');

    if numElements <= 0 then
      boundsCheckHalt('Cannot move-initialize array because number of ' +
                      'elements to copy is <= 0');

    const order = a.domain.indexOrder(idx);
    const hi = order + numElements;

    if hi > a.size then
      boundsCheckHalt('Cannot move-initialize array because one or ' +
                      'more indices are outside its domain');
  }

  private inline proc _errorNonCopyableType(type t) {
    if !isCopyableType(t) then
      compilerError('Cannot move-initialize array with non-copyable ',
                    'element type: ', t:string, 2);
  }

  // TODO: Relax this restriction in the future.
  private inline proc _errorNot1DRectangularArray(a) {
    if !a.isDefaultRectangular() || a.rank > 1 then
      compilerError('Can only move-initialize one-dimensional ',
                    'rectangular arrays', 2);
  }

  /*
    Move-initialize a group of array elements from a different group of
    elements in the same array.

    :arg a: An array to move-initialize
    :type a: `[]`

    :arg dstStartIndex: Destination index of elements to move-initialize
    :type dstStartIndex: `a.idxType`

    :arg srcStartIndex: Source index of elements to move-initialize from
    :type srcStartIndex: `a.idxType`

    :arg numElements: The number of elements to move-initialize
    :type numElements: int
  */
  proc moveInitializeArrayElements(ref a: [?d], dstStartIndex: a.idxType,
                                   srcStartIndex: a.idxType,
                                   numElements: int) {
    _errorNonCopyableType(a.eltType);
    _errorNot1DRectangularArray(a);

    if boundsChecking { 
      _haltBadElementRange(a, dstStartIndex, numElements);
      _haltBadIndex(a, dstStartIndex, 'dstStartIndex');
      _haltBadIndex(a, srcStartIndex, 'srcStartIndex');
    }

    if dstStartIndex == srcStartIndex then
      return;

    const d = a.domain;

    const dstLo = d.indexOrder(dstStartIndex);
    const srcLo = d.indexOrder(srcStartIndex);

    forall i in 0..<numElements {
      const dstIdx = d.orderToIndex(dstLo + i);
      const srcIdx = d.orderToIndex(srcLo + i);
      ref dst = a[dstIdx];
      ref src = a[srcIdx];
      moveInitialize(dst, src);
    }
  }

  /*
    Move-initialize a group of array elements from a different group of
    elements in another array.

    :arg dstA: An array to move-initialize
    :type dstA: `[] ?t`

    :arg dstStartIndex: Destination index of elements to move-initialize
    :type dstStartIndex: ``dstA.idxType``

    :arg srcA: An array to move-initialize from
    :type srcA: `[] t`

    :arg srcStartIndex: Source index of elements to move-initialize from
    :type srcStartIndex: ``srcA.idxType``

    :arg numElements: The number of elements to move-initialize
    :type numElements: int
  */
  proc moveInitializeArrayElements(ref dstA: [] ?t,
                                   dstStartIndex: dstA.idxType,
                                   srcA: [] t,
                                   srcStartIndex: srcA.idxType,
                                   numElements: int) {
    _errorNot1DRectangularArray(dstA);
    _errorNot1DRectangularArray(srcA);
    _errorNonCopyableType(t);

    if boundsChecking {
      _haltBadElementRange(dstA, dstStartIndex, numElements);
      _haltBadElementRange(srcA, srcStartIndex, numElements);
      _haltBadIndex(dstA, dstStartIndex, 'dstStartIndex');
      _haltBadIndex(srcA, srcStartIndex, 'srcStartIndex');
    }

    const isSameArray = dstA._instance == srcA._instance;

    if isSameArray && dstStartIndex == srcStartIndex then
      return;

    const dstD = dstA.domain;
    const srcD = srcA.domain;

    var dstLo = dstD.indexOrder(dstStartIndex);
    var srcLo = dstD.indexOrder(srcStartIndex);

    // TODO: Optimize communication for this loop?
    forall i in 0..<numElements {
      const dstIdx = dstD.orderToIndex(dstLo + i);
      const srcIdx = srcD.orderToIndex(srcLo + i);
      ref dst = dstA[dstIdx];
      ref src = srcA[srcIdx];
      moveInitialize(dst, src);
    }
  }

// Memory.Initialization;
}

