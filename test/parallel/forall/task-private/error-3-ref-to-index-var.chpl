// The compiler must ensure that a task-private variable
// does not reference the index variable of the loop.
// Ditto for task startup/teardown blocks, when we have them.
//
// At the moment the error message is not ideal.
// Instead it should state the above explicitly.

forall lol in Locales with (var x = lol.id) do
  writeln(lol, x);
