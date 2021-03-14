var A: [1..3] int;
var B: [1..4] int = [1,2,3,4];

//
// Iterate over tuple of arrays: ab should take on A then B
//
for ab in (A,B) do
  writeln(ab);


//
// For someone who forgets zip() they may type this:  Why
// can A and B be de-tupled into (a,b)?  Seems like a bug.
// Is it because we permit tuples to be assigned to arrays
// still?  I'll investigate.
//
for (a,b) in (A,B) do
  writeln((a,b));
