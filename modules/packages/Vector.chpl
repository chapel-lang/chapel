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

// TODO:
// Design discussions:
// - when removing, should the removed element be deinitialized?
// - when shuffling elements around (during resize, or insertion) should we
//   use PRIM_MOVE or assignment?
// - how should error handling be used?
// - name of type (e.g. 'list' vs. 'vector')
// - use 'uint' as idxType?
// - support slicing?
//
// Implementation:
// - parallel safety
// - lifetime clauses (encountered internal compiler error)
// - shrink memory allocation
// - support 'readThis'

record vector {
  type eltType;

  var _dom : domain(1);
  var _data : [_dom] eltType;
  var _live : range = 1..0; // represents 'live' indices in '_dom'

  proc init(type eltType) {
    this.eltType = eltType;
  }

  proc init(data : [] ?T) {
    this.eltType = T;
    this.complete();
    for d in data do push_back(d);
  }

  proc init(vec : vector(?T)) {
    this.eltType = T;
    this.complete();
    for v in vec do push_back(v);
  }

  proc init(type eltType, capacity : integral) {
    this.eltType = eltType;
    this.complete();
    this.requestCapacity(capacity);
  }

  proc chpl__promotionType() type return eltType;

  inline proc size return _live.size;
  inline proc isEmpty() : bool return _live.size == 0;

  proc front() : eltType {
    if isEmpty() then halt("front() called on empty vector");
    return _data[_live.first];
  }
  proc back() : eltType {
    if isEmpty() then halt("back() called on empty vector");
    return _data[_live.last];
  }

  pragma "no doc"
  proc reserveSpace(count : integral, backward : bool) {
    if count <= 0 then halt("vector.reserveSpace called with value <= 0");
    
    if backward {
      if this._live.last + count <= _dom.last then
        return;
    } else if this._live.first - count >= _dom.first then
      return;

    const newSize = max(_dom.size + 1, _dom.size * 1.2, _live.size + count):int;

    if backward then
      _dom = {_dom.first..#newSize};
    else
      _dom = {.._dom.last # -newSize};
  }

  proc push_back(in val: eltType) {
    reserveSpace(1, true);
    this._data[this._live.last+1] = val;
    this._live = this._live.first..(this._live.last+1);
  }

  // TODO: allow varargs
  // TODO: where isIterable(T)
  proc push_back(values : ?T)
  where isArray(T) || T == vector(eltType) {
    reserveSpace(values.size, true);
    const newSpace = (this._live.last + 1)..#values.size;
    for (i, v) in zip(newSpace, values) do
      _data[i] = v;
    this._live = this._live.first..newSpace.last;
  }

  proc pop_back() : eltType {
    if isEmpty() then halt("pop_back() called on empty vector");
    this._live = this._live.first.._live.last-1;
    return _data[_live.last + 1];
  }

  proc push_front(in val : eltType) {
    reserveSpace(1, false);
    _data[_live.first-1] = val;
    _live = (_live.first-1).._live.last;
  }
  proc push_front(values : ?T)
  where isArray(T) || T == vector(eltType) {
    reserveSpace(values.size, false);
    const newSpace = ..(this._live.first-1) # -values.size;
    for (i, v) in zip(newSpace, values) do
      _data[i] = v;
    _live = newSpace.first.._live.last;
  }

  proc pop_front() : eltType {
    if isEmpty() then halt("pop_front() called on empty vector");
    _live = (_live.first + 1).._live.last;
    return _data[_live.first - 1];
  }

  proc insert(idx : _dom.idxType, in val : eltType) {
    if isEmpty() && idx == 0 {
      push_back(val);
      return;
    } else if idx == this.size {
      push_back(val);
      return;
    } else if idx < 0 || idx > this.size then
      halt("vector.insert called with invalid index '" + idx:string + "' which must be in range '" + (0..this.size):string + "'");

    reserveSpace(1, true);
    const li = _live.first + idx;
    for i in li.._live.last by -1 do
      _data[i+1] = _data[i];
    _live = _live.first.._live.last+1;
    _data[li] = val;
  }
  proc insert(idx : _dom.idxType, values : ?T)
  where isArray(T) || T == vector(eltType) {
    if isEmpty() {
      if idx != 0 then halt("calling 'insert' on empty vector only accepts '0' as a valid index");
      push_back(values);
      return;
    } else if idx == this.size {
      push_back(values);
      return;
    } else if idx < 0 || idx > this.size then
      halt("vector.insert called with invalid index '" + idx:string + "' which must be in range '" + (0..this.size):string + "'");

    reserveSpace(values.size, true);
    const li = _live.first + idx;
    for i in li.._live.last by -1 do
      _data[i+values.size] = _data[i];
    _live = _live.first.._live.last+values.size;
    for (i, val) in zip(li..#values.size, values) {
      _data[i] = val;
    }
  }

  // TODO: Should these return something?
  proc remove(idx : _dom.idxType) {
    if isEmpty() then halt("cannot remove elements from an empty vector");
    else if idx < 0 || idx >= this.size then
      halt("vector.insert called with invalid index '" + idx:string + "' which must be in range '" + (0..#this.size):string + "'");
    // TODO: shrink memory allocation
    if idx != _live.last {
      const li = _live.first + idx;
      for i in li+1.._live.last do
        _data[i-1] = _data[i];
    }
    _live = _live.first.._live.last-1;
  }

  // Supports strided and unbounded ranges
  proc remove(rng : range(_dom.idxType, ?, ?)) {
    const translated = _live.first + rng;
    const actual = _live[translated]; // slice in case of unbounded rng
    if rng.boundedType != BoundedRangeType.bounded {
      if rng.boundedType == BoundedRangeType.boundedLow &&
         (rng.first < 0 || rng.first >= this.size) then
        halt("vector.remove called with invalid lower-bounded range: bound must be within '" + (0..#this.size):string + "'");
      else if rng.boundedType == BoundedRangeType.boundedHigh &&
        (rng.last < 0 || rng.last >= this.size) then
        halt("vector.remove called with invalid upper-bounded range: bound must be within '" + (0..#this.size):string + "'");
      else if actual.size == 0 then
        halt("vector.remove called with invalid unbounded range");
    } else if rng.size == 0 || actual.size != rng.size then
      halt("vector.remove called with invalid range: must be within '" + (0..#this.size):string + "'");

    if actual == _live {
      clear();
      return;
    }

    if actual.stridable == false || actual.stride == 1 {
      for i in actual.last+1.._live.last do
        _data[i-actual.size] = _data[i];
    } else {
      var cur = actual.first;
      for i in actual {
        for j in _live[i+1..#actual.stride-1] {
          _data[cur] = _data[j];
          cur += 1;
        }
      }
    }
    _live = _live.first..#(_live.size-actual.size);
  }

  proc reverse() {
    for i in 0..#(_live.size/2) {
      var temp = _data[_live.first+i];
      _data[_live.first+i] = _data[_live.last-i];
      _data[_live.last-i] = temp;
    }
  }

  proc clear() {
    _live = 1..0;
  }

  proc find(val : eltType) : (bool, _dom.idxType) {
    for i in _live do
      if _data[i] == val then return (true, i-_live.first);
    
    return (false, 0);
  }

  proc this(idx : _dom.idxType) ref : eltType {
    if boundsChecking && (idx < 0 || idx >= this.size) then
      halt("out of bounds: ", idx);
    return _data[_live.first + idx];
  }
  proc this(idx : _dom.idxType) const ref : eltType
  where shouldReturnRvalueByConstRef(eltType) {
    if boundsChecking && (idx < 0 || idx >= this.size) then
      halt("out of bounds");
    return _data[_live.first + idx];
  }
  proc this(idx : _dom.idxType) : eltType
  where shouldReturnRvalueByValue(eltType) {
    if boundsChecking && (idx < 0 || idx >= this.size) then
      halt("out of bounds");
    return _data[_live.first + idx];
  }

  proc this(d : domain) {
    compilerError("The vector type does not support slicing");
  }
  proc this(d : range(?,?,?)) {
    compilerError("The vector type does not support slicing");
  }

  iter these() ref : eltType {
    for i in _live do yield _data[i];
  }
  iter these(param tag:iterKind) ref : eltType where tag == iterKind.standalone {
    forall i in _live do yield _data[i];
  }
  iter these(param tag:iterKind) where tag == iterKind.leader {
    for follow in (0..#this.size).these(iterKind.leader) do yield follow;
  }
  iter these(param tag:iterKind, followThis) ref where tag == iterKind.follower {
    for f in followThis(1) do yield _data[_live.first+f];
  }

  proc writeThis(f) {
    _data[_live].writeThis(f);
  }
  proc readThis(f) {
    halt("reading a vector is not supported");
  }

  proc requestCapacity(capacity : integral) {
    reserveSpace(capacity, true);
  }

  proc equals(other : vector(eltType)) {
    return && reduce (this == other);
  }
  proc equals(other : [] eltType) {
    return && reduce (this == other);
  }
}
