// forall-statement

use iters;

// error - parallel iterator is not provided
forall IND in iter1ser() do
  writeln("forall IND in iter1ser");
