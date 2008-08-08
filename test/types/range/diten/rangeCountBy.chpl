module rangeCountStride {
  def pound(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.boundedLow {
    if i < 0 then
      halt("range cannot have a negative number of elements");
    if i == 0 then
      return new range(eltType = r.eltType,
                       boundedType = BoundedRangeType.bounded,
                       stridable = r.stridable,
                       _low = r.low + 1,
                       _high = r.low,
                       _stride = r.stride);

    return new range(eltType = r.eltType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = r.low,
                     _high = r.low + i*r.stride - 1,
                     _stride = r.stride);
  }

  def pound(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.boundedHigh {
    if i < 0 then
      halt("range cannot have a negative number of elements");
    if i == 0 then
      return new range(eltType = r.eltType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = r.high,
                     _high = r.high - 1,
                     _stride = r.stride);

    return new range(eltType = r.eltType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = if !r.stridable then r.high-i+1 else r.high+i*r.stride+1,
                     _high = r.high,
                     _stride = r.stride);
  }

  def pound(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.bounded {
    if i < 0 then
      halt("range cannot have a negative number of elements");
    if i > r.length then
      halt("bounded range is too small to access ", i, "elements");
    return new range(eltType = r.eltType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = if r.stride < 0 then r.high + r.stride*i + 1 else r.low,
                     _high = if r.stride < 0 then r.high else r.low + r.stride*i - 1,
                     _stride = r.stride);
  }

  def pound(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.boundedNone{
    halt("cannot use # operator on an unbounded range");
  }

  def by(r:range(?), i: int)
    where r.boundedType == BoundedRangeType.boundedLow {
    if i == 0 then
      halt("range cannot have 0 stride");
    if i < 0 then
      halt("range without high bound cannot have negative stride");
    return new range(eltType = r.eltType,
                     boundedType = r.boundedType,
                     stridable = true,
                     _low = r.low,
                     _stride = r.stride * i);
  }

  def by(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.boundedHigh {
    if i == 0 then
      halt("range cannot have 0 stride");
    if i > 0 then
      halt("range without low bound cannot have positive stride");
    return new range(eltType = r.eltType,
                     boundedType = r.boundedType,
                     stridable = true,
                     _high = r.high,
                     _stride = if r.stride == 1 then i else -(r.stride * i));
  }

  def by(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.bounded {
    if i == 0 then
      halt("range cannot have 0 stride");
    return new range(eltType = r.eltType,
                     boundedType = r.boundedType,
                     stridable = true,
                     _low = r.low,
                     _high = r.high,
                     _stride = r.stride * i);
  }

  def by(r:range(?), i:int)
    where r.boundedType == BoundedRangeType.boundedNone {
    halt("cannot stride unbounded range");                   
  }
}

def rangewrite(r: range(?)) {
  for i in r do
    write(i, " ");
  writeln();
}

def main {
  use rangeCountStride;
  rangewrite(pound(..10, 3));        // ..10 # 3
  rangewrite(pound(..10, 3) by 2);   // ..10 # 3 by 2
  rangewrite(pound(..10, 3) by -2);  // ..10 # 3 by -2
  rangewrite(pound(0.., 3));         // 0.. # 3
  rangewrite(pound(0.., 3) by 2);    // 0.. # 3 by 2
  rangewrite(pound(0.., 3) by -2);   // 0.. # 3 by -2
  rangewrite(pound(..10 by -2, 3));  // ..10 by -2 # 3
  rangewrite(pound(0.. by 2, 3));    // 0.. by 2 # 3
  rangewrite(pound(1..10, 3));       // 1..10 # 3
  rangewrite(pound(1..10 by -1, 3)); // 1..10 by -1 # 3
}
