/*
 Tuple unpacking should use at least one element, not ignore all with '(_, _)'.
*/

var tuples = [(1, 2), (3, 4), (5, 6)];

// All elements are ignored, the unpacking serves no purpose.
for (_, _) in tuples {
  writeln("processing");
}

// At least one element used.
for (i, _) in tuples {
  writeln(i);
}

// preferred: we can skip unpacking
for tuples {
  writeln("processing");
}
