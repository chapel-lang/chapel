proc test(ranges: range(?IT, boundKind.both, ?SB) ...?rank)
        : rank * range(IT, boundKind.both, false)
{
  var result: rank * range(IT, boundKind.both, false);
  return result;
}

writeln(test(1..5 by 2, 1..5 by 2));
