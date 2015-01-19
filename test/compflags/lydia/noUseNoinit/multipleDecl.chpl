// This test would not be valid without --no-use-noinit (because the use of
// uninitialized variables prior to their first assignment is discouraged)
var foo, bar, baz: int = noinit;
writeln(foo + bar + baz);
