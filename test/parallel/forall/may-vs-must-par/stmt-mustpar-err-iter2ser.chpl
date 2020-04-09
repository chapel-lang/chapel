// forall-statement

use iters;

// error - parallel iterator is not provided
forall IND in zip(iter1ser(), iter2ser()) do
  writeln("forall IND in zip(iter1ser(), iter2ser())");
