/*
 * Copyright 2004-2016 Cray Inc.
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

// ChapelReduce.chpl
//
module ChapelReduce {
  
  iter chpl__scanIteratorZip(op, data) {
    for e in zip((...data)) {
      op.accumulate(e);
      yield op.generate();
    }
    delete op;
  }
  
  iter chpl__scanIterator(op, data) {
    for e in data {
      op.accumulate(e);
      yield op.generate();
    }
    delete op;
  }
  
  proc chpl__reduceCombine(globalOp, localOp) {
    on globalOp {
      globalOp.lock();
      globalOp.combine(localOp);
      globalOp.unlock();
    }
  }

  inline proc chpl__cleanupLocalOp(globalOp, localOp) {
    // should this be part of chpl__reduceCombine ?
    delete localOp;
  }
  
  proc chpl__sumType(type eltType) type {
    var x: eltType;
    if isArray(x) {
      type xET = x.eltType;
      type xST = chpl__sumType(xET);
      if xET == xST then
        return eltType;
      else
        return [x.domain] xST;
    } else {
      return (x + x).type;
    }
  }
  
  pragma "ReduceScanOp"
  class ReduceScanOp {
    var lock$: sync bool;
    proc lock() {
      lock$.writeEF(true);
    }
    proc unlock() {
      lock$.readFE();
    }
  }
  
  class SumReduceScanOp: ReduceScanOp {
    type eltType;
    var value: chpl__sumType(eltType);

    // Rely on the default value of the desired type.
    // Todo: is this efficient when that is an array?
    proc identity {
      var x: chpl__sumType(eltType); return x;
    }
    proc accumulate(x) {
      value += x;
    }
    proc combine(x) {
      value += x.value;
    }
    proc generate() return value;
    proc clone() return new SumReduceScanOp(eltType=eltType);
  }
  
  class ProductReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _prod_id(eltType);
    proc accumulate(x) {
      value *= x;
    }
    proc combine(x) {
      value *= x.value;
    }
    proc generate() return value;
    proc clone() return new ProductReduceScanOp(eltType=eltType);
  }
  
  class MaxReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return min(eltType);
    proc accumulate(x) {
      value = max(x, value);
    }
    proc combine(x) {
      value = max(value, x.value);
    }
    proc generate() return value;
    proc clone() return new MaxReduceScanOp(eltType=eltType);
  }
  
  class MinReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return max(eltType);
    proc accumulate(x) {
      value = min(x, value);
    }
    proc combine(x) {
      value = min(value, x.value);
    }
    proc generate() return value;
    proc clone() return new MinReduceScanOp(eltType=eltType);
  }
  
  class LogicalAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _land_id(eltType);
    proc accumulate(x) {
      value &&= x;
    }
    proc combine(x) {
      value &&= x.value;
    }
    proc generate() return value;
    proc clone() return new LogicalAndReduceScanOp(eltType=eltType);
  }
  
  class LogicalOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _lor_id(eltType);
    proc accumulate(x) {
      value ||= x;
    }
    proc combine(x) {
      value ||= x.value;
    }
    proc generate() return value;
    proc clone() return new LogicalOrReduceScanOp(eltType=eltType);
  }
  
  class BitwiseAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _band_id(eltType);
    proc accumulate(x) {
      value &= x;
    }
    proc combine(x) {
      value &= x.value;
    }
    proc generate() return value;
    proc clone() return new BitwiseAndReduceScanOp(eltType=eltType);
  }
  
  class BitwiseOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _bor_id(eltType);
    proc accumulate(x) {
      value |= x;
    }
    proc combine(x) {
      value |= x.value;
    }
    proc generate() return value;
    proc clone() return new BitwiseOrReduceScanOp(eltType=eltType);
  }
  
  class BitwiseXorReduceScanOp: ReduceScanOp {
    type eltType;
    var value = identity;
  
    proc identity return _bxor_id(eltType);
    proc accumulate(x) {
      value ^= x;
    }
    proc combine(x) {
      value ^= x.value;
    }
    proc generate() return value;
    proc clone() return new BitwiseXorReduceScanOp(eltType=eltType);
  }
  
  class maxloc: ReduceScanOp {
    type eltType;
    var value = identity;
    var uninitialized = true;
  
    proc identity return min(eltType);
    proc accumulate(x) {
      if uninitialized || (x(1) > value(1)) ||
        ((x(1) == value(1)) && (x(2) < value(2))) then
        value = x;
      uninitialized = false;
    }
    proc combine(x) {
      if uninitialized || (x.value(1) > value(1)) ||
        ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
        if !x.uninitialized {
          value = x.value;
          uninitialized = false;
        }
      }
    }
    proc generate() return value;
    proc clone() return new maxloc(eltType=eltType);
  }
  
  class minloc: ReduceScanOp {
    type eltType;
    var value = identity;
    var uninitialized = true;
  
    proc identity return max(eltType);
    proc accumulate(x) {
      if uninitialized || (x(1) < value(1)) ||
        ((x(1) == value(1)) && (x(2) < value(2))) then
        value = x;
      uninitialized = false;
    }
    proc combine(x) {
      if uninitialized || (x.value(1) < value(1)) ||
        ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
        if !x.uninitialized {
          value = x.value;
          uninitialized = false;
        }
      }
    }
    proc generate() return value;
    proc clone() return new minloc(eltType=eltType);
  }
  
}
