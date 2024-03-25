
/*****
      ex_stmt.chpl -
      Examples of statements.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/


writeln("Demonstrating while statements");
/* This does not execute. */
var i = -1;
while ((0 <= i) && (i < 5)) {
  writef("  %i", i);
  i += 1;
}
writef("\n");

/* This does. */
i = -1;
do {
  const j = i + 1;
  writef("  %i", i);
  i += 1;
} while ((0 <= j) && (j < 5));
writef("\n");

writeln("\nDemonstrating labeled for statements");
label outer
for y in 1..5 {
  for x in 1..5 {
    if (x == y) {
      writef("\n");
      continue outer;
    } else {
      writef("  (%i,%i)", x,y);
    }
  }
}
writef("\n");
