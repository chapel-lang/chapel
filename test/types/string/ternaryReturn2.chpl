// This test is to lock in behavior around a bug involving returning strings
// from nested ternary returns

// This is not the version that failed, it was added to ensure that the cases
// that worked before are not broken by the solution
proc positive(z : string) {
  param EMPTY = '';
  const temp = if z == '+' then z else EMPTY;
  return if z == '~' then ' ' else temp;
}

const s = positive('+');
writeln(s);
