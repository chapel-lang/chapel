// This test is to lock in a bug involving returning strings from nested ternary
// returns

// This is the version that originally failed.  Please do not modify the return
// statement, to ensure the test continues to lock in any future fix
proc positive(z : string) {
  param EMPTY = '';
  return if z == '~' then ' ' else (if z == '+' then z else EMPTY);
}

const s = positive('+');
writeln(s);
