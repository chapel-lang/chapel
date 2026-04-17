// Test error message for undefined outer variable in coforall with-clause
// Issue #25639

coforall i in 1..2 with (ref undefinedVar) {
  undefinedVar += i;
}
