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

/* Support for move-initializing and deinitializing values.

  The :mod:`MemMove` module provides functions which enable users to
  move-initialize and deinitialize values.

  The functionality provided by this module can be used to implement
  collections in a manner similar to those implemented by the Chapel standard
  modules (such as :mod:`List` or :mod:`Map`).

  .. note::

    Throughout documentation, the term `variable` also includes non-const
    formals, array elements, record or class fields, and tuple components.


*/
module MemMove {

  // Mark as "unsafe" to silence lifetime errors.
  pragma "unsafe"
  private inline proc _move(ref dst, const ref src) {
    __primitive("=", dst, src);
  }

  /*
    Check to see if a given type would normally be destroyed automatically
    by the compiler when its lifetime ends. For example, passing an ``owned``
    class would see this function return ``true``, whereas passing an
    ``unmanaged`` class would result in ``false``.

    A ``shared`` class would also result in ``true`` because the compiler
    destroys ``shared`` variables to decrement the reference count (and
    possibly the object being managed, as well).

    :arg t: A type to check
    :type t: `type`

    :return: ``true`` if ``t`` needs to be automatically destroyed
    :rtype: param bool
  */
  proc needsDestroy(type t) param : bool {
    return __primitive("needs auto destroy", t);
  }

  /*
    Explicitly destroy a variable as the compiler would when its lifetime ends.
    The variable referred to by ``obj`` should be considered unusable after a
    call to this function, and its particular state is undefined.

    This function has no effect if :proc:`needsDestroy` returns ``false`` for
    the argument's type.

    .. warning::

      At present the compiler does not account for manual destruction performed
      upon a call to :proc:`destroy()`. It should only be called when automatic
      destruction would not occur otherwise.

    :arg obj: A variable to deinitialize
  */
  proc destroy(ref obj: ?t) {
    if needsDestroy(t) then
      chpl__autoDestroy(obj);
  }

  /*
    Move-initialize ``dst`` with the value in ``src``. The contents of ``dst``
    are not deinitialized before the move, and ``src`` is not deinitialized
    after the move.

    .. warning::

      If ``dst`` references an already initialized variable, it will be
      overwritten by the contents of ``src`` without being deinitialized
      first. Call :proc:`destroy()` to deinitialize ``dst`` if
      necessary.

    .. warning::

      The static types of ``dst`` and ``src`` must match, or else a
      compile-time error will be issued.

    .. note::

      If the compiler inserts a copy for the argument to ``src``, then a
      compile-time error will be issued. The most likely cause is that the
      argument is used elsewhere following the call to ``moveInitialize``.
      The ``moveFrom`` function can be used with the ``src`` argument to avoid
      the copy when certain of the variable's usage:

      moveInitialize(myDst, moveFrom(mySrc));

    :arg dst: A variable to move-initialize, whose type matches ``src``

    :arg src: A value to move-initialize from
  */
  proc moveInitialize(ref dst,
                      pragma "no auto destroy"
                      pragma "error on copy" in src) {
    if __primitive("static typeof", dst) != __primitive("static typeof", src) {
      compilerError("type mismatch move-initializing an expression of type '"+dst.type:string+"' from one of type '"+src.type:string+"'");
    } else if __primitive("static typeof", dst) != nothing {
      _move(dst, src);
    }
  }

  /*
    Move the contents of the variable or constant referred to by ``src`` into
    a new returned value.

    .. warning::

      The variable or constant referred to by ``src`` should be considered
      uninitialized after a call to this function.

    :arg src: A variable or constant to move

    :return: The contents of ``src`` moved into a new value
  */
  proc moveFrom(const ref src: ?t) {
    // Note: using 't' as an explicit return type can result in incorrect
    // behavior for arrays
    if t == nothing {
      return none;
    } else {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var result: t;
      _move(result, src);
      return result;
    }
  }

  /*
    Swap the contents of the variables referred to by ``x`` and ``y``.
    This function does not call the ``<=>`` operator. Unlike the ``<=>``
    operator, :proc:`moveSwap()` does not perform assignment or
    initialization.

    :arg x: A variable to swap

    :arg y: A variable to swap
  */
  proc moveSwap(ref x: ?t, ref y: t) {
    if t != nothing {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var temp: t;
      _move(temp, x);
      _move(x, y);
      _move(y, temp);
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
                      'more indices fall outside its domain');
  }

  private inline proc _haltRangeOverlap(dstIndex, srcIndex, numElements) {
    import HaltWrappers.boundsCheckHalt;

    const dstRange = dstIndex..#numElements;
    const srcRange = srcIndex..#numElements;

    if dstRange[srcRange].size != 0 then
      boundsCheckHalt('Cannot move-initialize array because source and ' +
                      'destination ranges intersect');
  }

  // TODO: Relax this restriction in the future.
  private inline proc _errorNot1DRectangularArray(a) {
    if !a.isDefaultRectangular() || a.rank > 1 then
      compilerError('Can only move-initialize one-dimensional ',
                    'rectangular arrays', 2);
  }

  private proc _testArrayAlias(const dst, const dstRegion,
                               const src, const srcRegion) throws {
    // TODO: will not catch reindexes
    const actualDst = chpl__getActualArray(dst);
    const actualSrc = chpl__getActualArray(src);
    if actualDst == actualSrc {
      var overlap = false;
      if isRange(dstRegion) then
        overlap = ({dstRegion})[srcRegion].isEmpty() == false;
      else
        overlap = dstRegion[srcRegion].isEmpty() == false;

      if overlap {
        use IO;
        throw new IllegalArgumentError("Arguments to 'moveArrayElements' alias the same data. Regions are '%?' and '%?'".format(dstRegion, srcRegion));
      }
    }
  }

  //
  // Performs a number of compile-time checks for ``moveArrayElements``, and
  // also throws some errors for illegal argument conditions.
  //
  private proc _checkArgs(const dst, const dstRegion, const src, const srcRegion) throws {

    proc _checkIsRectangular(arg, param name : string) {
      if !arg.isRectangular() then
        compilerError("The '" + name + "' for 'moveArrayElements' must be rectangular", 3);
    }

    proc _isDomOrRange(arg) param : bool {
      return isDomain(arg) || isRange(arg);
    }

    _checkIsRectangular(dst, "dst");
    _checkIsRectangular(src, "src");

    if !_isDomOrRange(dstRegion) || !_isDomOrRange(srcRegion) {
      compilerError("Region arguments to 'moveArrayElements' must be ranges or rectangular domains", 2);
    }
    if isDomain(dstRegion) then _checkIsRectangular(dstRegion, "dstRegion");
    if isDomain(srcRegion) then _checkIsRectangular(srcRegion, "srcRegion");

    if (isRange(dstRegion) && dst.rank > 1) ||
       (isRange(srcRegion) && src.rank > 1) then
      compilerError("'moveArrayElements' does not accept range regions for arrays with more than 1 dimension", 2);

    if (isRange(dstRegion) && dstRegion.bounds != boundKind.both) ||
       (isRange(srcRegion) && srcRegion.bounds != boundKind.both) then
      compilerError("'moveArrayElements' does not accept unbounded ranges", 2);

    proc _idxHelper(A, B, param Aname: string, param Bname: string) {
      if A.idxType != B.idxType {
        param args = "'" + Aname + "' and '" + Bname + "'";
        param types = "'" + A.idxType:string + "' vs. '" + B.idxType:string + "'";
        compilerError(args + " for 'moveArrayElements' must have the same index type: " + types, 3);
      }
    }
    _idxHelper(dst, dstRegion, "dst", "dstRegion");
    _idxHelper(src, srcRegion, "src", "srcRegion");

    // Runtime checks
    if dstRegion.size != srcRegion.size {
      throw new IllegalArgumentError("Destination and source specify a different number of elements to move: " + dstRegion.size:string + " vs. " + srcRegion.size:string);
    }

    const dstGood = dst.domain.contains(if isRange(dstRegion) then {dstRegion}
                                        else dstRegion);
    const srcGood = src.domain.contains(if isRange(srcRegion) then {srcRegion}
                                        else srcRegion);
    if !dstGood then
      throw new IllegalArgumentError("illegal argument 'dstRegion': region contains invalid indices");
    if !srcGood then
      throw new IllegalArgumentError("illegal argument 'srcRegion': region contains invalid indices");

    _testArrayAlias(dst, dstRegion, src, srcRegion);
  }

  /*
    Move-initialize a group of array elements from a group of elements in
    another array. This function is equivalent to a sequence of individual
    calls to :proc:`moveInitialize()`.

    This function only accepts rectangular arrays, rectangular domains, or
    ranges. The index type of the regions must also match the index type of
    their corresponding arrays. Any range arguments must have both an upper and
    lower bound.

    :arg dst: The destination array
    :arg dstRegion: A domain or range of indices in ``dst``
    :arg src: The source array
    :arg srcRegion: A domain or range of indices in ``src``

    :throws IllegalArgumentError: if region sizes do not match,
                                  if a region contains indices not in the
                                  corresponding array,
                                  if ``src`` and ``dst`` are determined to be
                                  aliases and the regions overlap.
  */
  @unstable("'moveArrayElements' is unstable and subject to change in the future")
  proc moveArrayElements(ref dst:[] ?eltType, const dstRegion,
                         const ref src:[] eltType, const srcRegion) : void throws {
    _checkArgs(dst, dstRegion, src, srcRegion);

    forall (di, si) in zip(dstRegion, srcRegion) with (ref dst) {
      moveInitialize(dst[di], moveFrom(src[si]));
    }
  }

  /*
    Move-initialize a group of array elements from a group of elements in
    another array. This function is equivalent to a sequence of individual
    calls to :proc:`moveInitialize()`.

    This function only accepts rectangular arrays.

    :arg dst: The destination array
    :arg src: The source array

    :throws IllegalArgumentError: if the array sizes do not match,
                                  if ``src`` and ``dst`` are determined to be
                                  aliases and overlap.
  */
  @unstable("'moveArrayElements' is unstable and subject to change in the future")
  proc moveArrayElements(ref dst:[] ?eltType, const ref src:[] eltType) : void throws {
    // Run the checks here so we get better line numbers for errors.
    _checkArgs(dst, dst.domain, src, src.domain);

    moveArrayElements(dst, dst.domain, src, src.domain);
  }

// MemMove;
}
