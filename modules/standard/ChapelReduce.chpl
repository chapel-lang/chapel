use List;
def _reduce(r, s) { // reduce s by reduction r
  for e in s do
    r.accumulate(e);
  return r.generate();  
}

def _scan(r, s) {
  var s2: list(r.eltType);
  for e in s {
    r.accumulate(e);
    s2.append(r.generate());
  }
  return s2;
}


def _sum_type(type eltType) {
  var x: eltType;
  return x + x;
}

class _sum {
  type eltType;
  var value : _sum_type(eltType).type;
  def accumulate(x) {
    value = value + x;
  }
  def generate() return value;
}

class _prod {
  type eltType;
  var value : eltType = _prod_id( eltType);

  def accumulate(x) {
    value = value * x;
  }
  def generate() return value;
}

class _max {
  type eltType;
  var value : eltType = min( eltType);

  def accumulate(x) {
    value = max(x, value);
  }
  def generate() return value;
}

class _min {
  type eltType;
  var value : eltType = max( eltType);

  def accumulate(x) {
    value = min(x, value);
  }
  def generate() return value;
}

class maxloc {
  type eltType;
  var value: eltType;
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || x(1) > value(1) then
      value = x;
    uninitialized = false;
  }
  def generate() return value;
}

class minloc {
  type eltType;
  var value: eltType;
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || x(1) < value(1) then
      value = x;
    uninitialized = false;
  }
  def generate() return value;
}

class _land {                 // logical and
  type eltType;
  var value : eltType = _land_id( eltType);

  def accumulate(x) {
    value = value && x;
  }
  def generate() return value;
}

class _lor {                 // logical or
  type eltType;
  var value : eltType = _lor_id( eltType);

  def accumulate(x) {
    value = value || x;
  }
  def generate() return value;
}

class _band {                 // bit-wise and
  type eltType;
  var value : eltType = _band_id( eltType);

  def accumulate(x) {
    value = value & x;
  }
  def generate() return value;
}

class _bor {                 // bit-wise or
  type eltType;
  var value : eltType = _bor_id( eltType);

  def accumulate(x) {
    value = value | x;
  }
  def generate() return value;
}

class _bxor {                // bit-wise xor
  type eltType;
  var value : eltType = _bxor_id( eltType);

  def accumulate(x) {
    value = value ^ x;
  }
  def generate() return value;
}
