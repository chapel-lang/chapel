def chpl__scanIterator(op, data) {
  for e in data {
    op.accumulate(e);
    yield op.generate();
  }
  delete op;
}

def chpl__reduceCombine(globalOp, localOp) {
  on globalOp {
    globalOp.lock();
    globalOp.combine(localOp);
    globalOp.unlock();
  }
}

def chpl__sumType(type eltType) type {
  var x: eltType;
  return (x + x).type;
}

class ReduceScanOp {
  var lock$: sync bool;
  def lock() {
    lock$.writeEF(true);
  }
  def unlock() {
    lock$.readFE();
  }
}

class SumReduceScanOp: ReduceScanOp {
  type eltType;
  var value: chpl__sumType(eltType);
  def accumulate(x) {
    value = value + x;
  }
  def combine(x) {
    value = value + x.value;
  }
  def generate() return value;
}

class ProductReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _prod_id(eltType);

  def accumulate(x) {
    value = value * x;
  }
  def combine(x) {
    value = value * x.value;
  }
  def generate() return value;
}

class MaxReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = min(eltType);

  def accumulate(x) {
    value = max(x, value);
  }
  def combine(x) {
    value = max(value, x.value);
  }
  def generate() return value;
}

class MinReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = max(eltType);

  def accumulate(x) {
    value = min(x, value);
  }
  def combine(x) {
    value = min(value, x.value);
  }
  def generate() return value;
}

class LogicalAndReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _land_id(eltType);

  def accumulate(x) {
    value = value && x;
  }
  def combine(x) {
    value = value && x.value;
  }
  def generate() return value;
}

class LogicalOrReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _lor_id(eltType);

  def accumulate(x) {
    value = value || x;
  }
  def combine(x) {
    value = value || x.value;
  }
  def generate() return value;
}

class BitwiseAndReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _band_id(eltType);

  def accumulate(x) {
    value = value & x;
  }
  def combine(x) {
    value = value & x.value;
  }
  def generate() return value;
}

class BitwiseOrReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _bor_id(eltType);

  def accumulate(x) {
    value = value | x;
  }
  def combine(x) {
    value = value | x.value;
  }
  def generate() return value;
}

class BitwiseXorReduceScanOp: ReduceScanOp {
  type eltType;
  var value : eltType = _bxor_id(eltType);

  def accumulate(x) {
    value = value ^ x;
  }
  def combine(x) {
    value = value ^ x.value;
  }
  def generate() return value;
}

class maxloc: ReduceScanOp {
  type eltType;
  var value: eltType = min(eltType);
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || (x(1) > value(1)) ||
      ((x(1) == value(1)) && (x(2) < value(2))) then
      value = x;
    uninitialized = false;
  }
  def combine(x) {
    if uninitialized || (x.value(1) > value(1)) ||
      ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
      if !x.uninitialized {
        value = x.value;
        uninitialized = false;
      }
    }
  }
  def generate() return value;
}

class minloc: ReduceScanOp {
  type eltType;
  var value: eltType = max(eltType);
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || (x(1) < value(1)) ||
      ((x(1) == value(1)) && (x(2) < value(2))) then
      value = x;
    uninitialized = false;
  }
  def combine(x) {
    if uninitialized || (x.value(1) < value(1)) ||
      ((x.value(1) == value(1)) && (x.value(2) < value(2))) {
      if !x.uninitialized {
        value = x.value;
        uninitialized = false;
      }
    }
  }
  def generate() return value;
}
