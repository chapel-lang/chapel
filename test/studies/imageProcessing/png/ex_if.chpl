
/*****
      ex_if.chpl -
      Example of an if statement using block and brace form.  See text for
      description.

      Call:
        ex_if

      c 2015-2018 Primordial Machine Vision Systems
*****/

const a = 1;
const b = 2;
const c = 1;

writeln("\nif block form:");
/* You can follow the 'then' keyword with only one statement.  The second
   line will always be printed. */
if (a == b) then
  writeln("a matches b");
writeln("going to next test");

/* Same with an else in block form, just one statement follwos it. */
if (a == c) then
  writeln("a matches c");
else
  writeln("a, c differ");
writeln("tests done");

writeln("\nif brace form:");
/* With braces you can group many statements.  From this group only the
   one statement about a and c matching will print. */
if (a == b) {
  writeln("a matches b");
  writeln("going to next test");
}
if (a == c) {
  writeln("a matches c");
} else {
  writeln("a, c differ");
  writeln("tests done");
 }

writeln("\nchained if's:");
/* With a chain of if's, the last else goes to the last if. */
if (a == b) then
  writeln("a matches b");
else if (a != c) then
  writeln("a, c differ");
else
  writeln("a matches c");
