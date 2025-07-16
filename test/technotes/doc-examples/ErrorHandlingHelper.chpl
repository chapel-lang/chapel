//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
// ErrorHandlingHelper.chpl
// Define some helper procedures used throughout the following examples...
//

inline proc canThrow(x) throws {
  if x > 0 then throw new Error('SOMETIMES');
}

inline proc alwaysThrows(): int throws {
  throw new Error('ALWAYS');
  return 0;
}
/* STOP_EXAMPLE_0 */
