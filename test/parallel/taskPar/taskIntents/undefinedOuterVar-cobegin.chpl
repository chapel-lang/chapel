// Test error message for undefined outer variable in cobegin with-clause
// Issue #25639

cobegin with (ref undefinedVar) {
  { undefinedVar = 1; }
  { undefinedVar = 2; }
}
