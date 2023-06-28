proc test(ranges: range(?IT, boundKind.both, ?SB) ...?rank)
        : rank * range(IT, boundKind.both)
{
  var result: rank * range(IT, boundKind.both);
  return result;
}

writeln(test(1..5 by 2, 1..5 by 2));
