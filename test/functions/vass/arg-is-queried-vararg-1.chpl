proc test(ranges: range(?IT, BoundedRangeType.bounded, ?SB) ...?rank)
        : rank * range(IT, BoundedRangeType.bounded, false)
{
  var result: rank * range(IT, BoundedRangeType.bounded, false);
  return result;
}

writeln(test(1..5 by 2, 1..5 by 2));
