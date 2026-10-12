// Test error message for undefined outer variable in begin with-clause
// Issue #25639

begin with (ref undefinedVar) {
  undefinedVar = 42;
}
