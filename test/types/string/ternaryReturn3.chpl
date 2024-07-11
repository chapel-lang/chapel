// This test is to lock in behavior around a bug involving returning strings
// from nested ternary returns

// This is not the version that failed, it was added to ensure that the cases
// that worked before were not broken by the solution
proc positive(z : string) {
  param EMPTY = '';
  return if z == '~' then ' ' else (if z == '+' then z:string else EMPTY);
}

const s = positive('+');
writeln(s);
