def _scan(r, s) {
  var i=1, size=4;
  var D = [1..size];
  var A: [D] r.eltType;
  for e in s {
    if i > size {
      size = size * 2;
      D = [1..size];
    }
    r.accumulate(e);
    A(i) = r.generate();
    i = i + 1;
  }
  D = [1..i-1];
  return A;
}

def _sum_type(type eltType) {
  var x: eltType;
  return x + x;
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
  var value : _sum_type(eltType).type;
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
  var value: eltType;
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
      value = x.value;
      uninitialized = x.uninitialized;
    }
  }
  def generate() return value;
}

class minloc: ReduceScanOp {
  type eltType;
  var value: eltType;
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
      value = x.value;
      uninitialized = x.uninitialized;
    }
  }
  def generate() return value;
}
