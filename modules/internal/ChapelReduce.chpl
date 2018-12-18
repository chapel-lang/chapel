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

// ChapelReduce.chpl
//
module ChapelReduce {
  use ChapelStandard;

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

  // Return true for simple cases where x.type == (x+x).type.
  // This should be true for the great majority of cases in practice.
  // This proc helps us avoid run-time computations upon chpl__sumType().
  // Which is important for costly cases ex. when 'eltType' is an array.
  // It also allows us to accept 'eltType' that is the result of
  // __primitive("static typeof"), i.e. with uninitialized _RuntimeTypeInfo.
  //
  proc chpl_sumTypeIsSame(type eltType) param {
    if isNumeric(eltType) || isString(eltType) {
      return true;

    } else if isDomain(eltType) {
      // Since it is a param function, this code will be squashed.
      // It will not execute at run time.
      var d: eltType;
      // + preserves the type for associative domains.
      // Todo: any other easy-to-compute cases?
      return isAssociativeDom(d);

    } else if isArray(eltType) {
      // Follow the lead of chpl_buildStandInRTT. Thankfully, this code
      // will not execute at run time. Otherwise we could get in trouble,
      // as "static typeof" produces uninitialized _RuntimeTypeInfo values.
      type arrInstType = __primitive("static field type", eltType, "_instance");
      var instanceObj: arrInstType;
      type instanceEltType = __primitive("static typeof", instanceObj.eltType);
      return chpl_sumTypeIsSame(instanceEltType);

    } else {
      // Otherwise, let chpl__sumType() deal with it.
      return false;
    }
  }

  proc chpl__sumType(type eltType) type {
   if chpl_sumTypeIsSame(eltType) {
    return eltType;
   } else {
    // The answer may or may not be 'eltType'.
    var x: eltType;
    if isArray(x) {
      type xET = x.eltType;
      type xST = chpl__sumType(xET);
      if xET == xST then
        return eltType;
      else
        return [x.domain] xST;
    } else {
      use Reflection;
      if ! canResolve("+", x, x) then
        // Issue a user-friendly error.
        compilerError("+ reduce cannot be used on values of the type ",
                      eltType:string);
      return (x + x).type;
    }
   }
  }

  pragma "ReduceScanOp"
  class ReduceScanOp {
    var l: chpl__processorAtomicType(bool); // only accessed locally

    proc lock() {
      var lockAttempts = 0,
          maxLockAttempts = (2**10-1);
      while l.testAndSet(memory_order_acquire) {
        lockAttempts += 1;
        if (lockAttempts & maxLockAttempts) == 0 {
          maxLockAttempts >>= 1;
          chpl_task_yield();
        }
      }
    }
    proc unlock() {
      l.clear(memory_order_release);
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
    proc accumulateOntoState(ref state, x) {
      state += x;
    }
    proc combine(x) {
      value += x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged SumReduceScanOp(eltType=eltType);
  }

  class ProductReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _prod_id(eltType);

    proc identity return _prod_id(eltType);
    proc accumulate(x) {
      value *= x;
    }
    proc accumulateOntoState(ref state, x) {
      state *= x;
    }
    proc combine(x) {
      value *= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged ProductReduceScanOp(eltType=eltType);
  }

  class MaxReduceScanOp: ReduceScanOp {
    type eltType;
    var value = min(eltType);

    proc identity return min(eltType);
    proc accumulate(x) {
      value = max(x, value);
    }
    proc accumulateOntoState(ref state, x) {
      state = max(state, x);
    }
    proc combine(x) {
      value = max(value, x.value);
    }
    proc generate() return value;
    proc clone() return new unmanaged MaxReduceScanOp(eltType=eltType);
  }

  class MinReduceScanOp: ReduceScanOp {
    type eltType;
    var value = max(eltType);

    proc identity return max(eltType);
    proc accumulate(x) {
      value = min(x, value);
    }
    proc accumulateOntoState(ref state, x) {
      state = min(state, x);
    }
    proc combine(x) {
      value = min(value, x.value);
    }
    proc generate() return value;
    proc clone() return new unmanaged MinReduceScanOp(eltType=eltType);
  }

  class LogicalAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _land_id(eltType);

    proc identity return _land_id(eltType);
    proc accumulate(x) {
      value &&= x;
    }
    proc accumulateOntoState(ref state, x) {
      state &&= x;
    }
    proc combine(x) {
      value &&= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged LogicalAndReduceScanOp(eltType=eltType);
  }

  class LogicalOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _lor_id(eltType);

    proc identity return _lor_id(eltType);
    proc accumulate(x) {
      value ||= x;
    }
    proc accumulateOntoState(ref state, x) {
      state ||= x;
    }
    proc combine(x) {
      value ||= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged LogicalOrReduceScanOp(eltType=eltType);
  }

  class BitwiseAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _band_id(eltType);

    proc identity return _band_id(eltType);
    proc accumulate(x) {
      value &= x;
    }
    proc accumulateOntoState(ref state, x) {
      state &= x;
    }
    proc combine(x) {
      value &= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged BitwiseAndReduceScanOp(eltType=eltType);
  }

  class BitwiseOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _bor_id(eltType);

    proc identity return _bor_id(eltType);
    proc accumulate(x) {
      value |= x;
    }
    proc accumulateOntoState(ref state, x) {
      state |= x;
    }
    proc combine(x) {
      value |= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged BitwiseOrReduceScanOp(eltType=eltType);
  }

  class BitwiseXorReduceScanOp: ReduceScanOp {
    type eltType;
    var value = _bxor_id(eltType);

    proc identity return _bxor_id(eltType);
    proc accumulate(x) {
      value ^= x;
    }
    proc accumulateOntoState(ref state, x) {
      state ^= x;
    }
    proc combine(x) {
      value ^= x.value;
    }
    proc generate() return value;
    proc clone() return new unmanaged BitwiseXorReduceScanOp(eltType=eltType);
  }

  proc _maxloc_id(type eltType) return (min(eltType(1)), max(eltType(2)));
  proc _minloc_id(type eltType) return max(eltType); // max() on both components

  class maxloc: ReduceScanOp {
    type eltType;
    var value = _maxloc_id(eltType);

    proc identity return _maxloc_id(eltType);
    proc accumulate(x) {
      if x(1) > value(1) ||
        ((x(1) == value(1)) && (x(2) < value(2))) then
        value = x;
    }
    proc combine(x) {
      if x.value(1) > value(1) ||
        ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
          value = x.value;
      }
    }
    proc generate() return value;
    proc clone() return new unmanaged maxloc(eltType=eltType);
  }

  class minloc: ReduceScanOp {
    type eltType;
    var value = _minloc_id(eltType);

    proc identity return _minloc_id(eltType);
    proc accumulate(x) {
      if x(1) < value(1) ||
        ((x(1) == value(1)) && (x(2) < value(2))) then
        value = x;
    }
    proc combine(x) {
      if x.value(1) < value(1) ||
        ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
          value = x.value;
      }
    }
    proc generate() return value;
    proc clone() return new unmanaged minloc(eltType=eltType);
  }

}
