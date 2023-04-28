proc test(ranges: ?rank * range(?IT, boundKind.both, ?SB))
        : rank * range(IT, boundKind.both, false)
{
  var result: rank * range(IT, boundKind.both, false);
  return result;
}

writeln(test((1..5 by 2, 1..5 by 2)));
