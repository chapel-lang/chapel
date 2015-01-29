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
  
  proc chpl__sumType(type eltType) type {
    var x: eltType;
    return (x + x).type;
  }
  
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
    proc accumulate(x) {
      value = value + x;
    }
    proc combine(x) {
      value = value + x.value;
    }
    proc generate() return value;
  }
  
  class ProductReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _prod_id(eltType);
  
    proc accumulate(x) {
      value = value * x;
    }
    proc combine(x) {
      value = value * x.value;
    }
    proc generate() return value;
  }
  
  class MaxReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = min(eltType);
  
    proc accumulate(x) {
      value = max(x, value);
    }
    proc combine(x) {
      value = max(value, x.value);
    }
    proc generate() return value;
  }
  
  class MinReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = max(eltType);
  
    proc accumulate(x) {
      value = min(x, value);
    }
    proc combine(x) {
      value = min(value, x.value);
    }
    proc generate() return value;
  }
  
  class LogicalAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _land_id(eltType);
  
    proc accumulate(x) {
      value = value && x;
    }
    proc combine(x) {
      value = value && x.value;
    }
    proc generate() return value;
  }
  
  class LogicalOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _lor_id(eltType);
  
    proc accumulate(x) {
      value = value || x;
    }
    proc combine(x) {
      value = value || x.value;
    }
    proc generate() return value;
  }
  
  class BitwiseAndReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _band_id(eltType);
  
    proc accumulate(x) {
      value = value & x;
    }
    proc combine(x) {
      value = value & x.value;
    }
    proc generate() return value;
  }
  
  class BitwiseOrReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _bor_id(eltType);
  
    proc accumulate(x) {
      value = value | x;
    }
    proc combine(x) {
      value = value | x.value;
    }
    proc generate() return value;
  }
  
  class BitwiseXorReduceScanOp: ReduceScanOp {
    type eltType;
    var value : eltType = _bxor_id(eltType);
  
    proc accumulate(x) {
      value = value ^ x;
    }
    proc combine(x) {
      value = value ^ x.value;
    }
    proc generate() return value;
  }
  
  class maxloc: ReduceScanOp {
    type eltType;
    var value: eltType = min(eltType);
    var uninitialized = true;
  
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
  }
  
  class minloc: ReduceScanOp {
    type eltType;
    var value: eltType = max(eltType);
    var uninitialized = true;
  
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
  }
  
}
