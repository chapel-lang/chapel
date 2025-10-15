// Test error message for undefined outer variable in forall with-clause
// Issue #25639

forall i in 1..2 with (ref undefinedVar) {
  undefinedVar += i;
}
