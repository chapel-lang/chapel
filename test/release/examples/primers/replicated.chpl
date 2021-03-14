// Replicated Distribution

/*
  This primer demonstrates uses of the Replicated Distribution.
  To use this distribution in a Chapel program, the following module must be
  used:
*/

  use ReplicatedDist;

// Preface
// -------
//
// For this example, we'll create a distributed domain and array
// and then initialize it just to give a brief flavor of how the
// distribution maps across locales. Running this example on 6
// locales does a nice job of illustrating the distribution
// characteristics.
//
// Like other distributions, Replicated supports options to map to a different
// virtual locale grid than the one used by default (a multidimensional
// factoring of the built-in ``Locales`` array), as well as
// to control the amount of parallelism used in data parallel
// loops.  For more details, see the documentation on
// :ref:`Standard Distributions <layouts_and_distributions>`.
//

//
// Make the program size configurable from the command line.
//
config const n = 8;

//
// Declare a 2-dimensional domain ``Space`` that we will later use to
// initialize the distributed domains.
//
const Space = {1..n, 1..n};


// Replicated
// ----------
//
// The ``Replicated`` distribution is different from other distributions:
// each of the original domain's indices is replicated onto
// each locale, as are the corresponding array elements.  For example,
// a domain ``{1..3}`` distributed using ``Replicated`` will store
// three indices per locale that the distribution is targeting (by
// default, all locales).  Similarly, an array declared over that
// domain will store three elements per locale.  Each locale's copy of
// the domain or array is known as its *replicand*.
//
// Consistency among these array replicands is NOT maintained
// automatically; users who want a replicated array to store the same
// values on every target locale will have to manage that consistency
// themselves.
//
// In general, operations on replicated domains and arrays only refer
// the local replicand.  The primary exception to this rule is the
// re-assignment of a replicated domain's indices.  In this case, the
// copy of the domain on each locale will be updated (and any arrays
// over the domain will be reallocated on each locale).
//
// Here's a declaration of a replicated domain and array:
//
const ReplicatedSpace = Space dmapped Replicated();
var RA: [ReplicatedSpace] int;

// Queries about the size of a replicated domain or array will return
// the size per locale:
//
writeln("Replicated Array has ", RA.size, " elements per locale");

//
// The following loop-based assignment to `RA` only affects the copy
// of 'RA' on the locale on which it's running (the last locale, in
// this example).  All other copies will remain in their
// default-initialized form.
//
on Locales[numLocales-1] do
  forall ra in RA do
    ra = here.id;

//
// Similarly, when reading the array, only the local copy will be
// accessed.  Thus, when running on more than one locale, the
// following writeln() will not see the modification performed by the
// loop above since the two statements are executed on distinct locales:
//
writeln("Locale 0's copy of RA is:\n", RA);


//
// To access the replicands owned by other locales, we can use the
// replicand method, which takes a locale as an argument and returns the
// array local to that locale:
//

writeln("Locale ", numLocales-1, "'s copy of RA is:\n",
         RA.replicand(Locales[numLocales-1]));

//
// Alternatively, we can print the replicated array from that locale:
//

on Locales[numLocales-1] do
  writeln("Locale ", numLocales-1, "'s copy of RA is:\n", RA);

//
// In order to see the replicands owned by all the locales, let's
// define and call a little utility function:
//
proc writeReplicands(X) {
  for loc in Locales {
    writeln(loc, ":");
    writeln(X.replicand(loc));
  }
}

writeln("Replicated Array Index Map");
writeReplicands(RA);
writeln();

//
// Whole-array assignment is similarly local only to the current
// locale's copy of the array:
//
var A: [Space] int = [(i,j) in Space] i*100 + j;
RA = A;
writeln("Replicated Array after whole-array assignment:");
writeReplicands(RA);
writeln();

//
// Here, we have each locale update its own copy of `RA` to store its
// locale ID, which results in a modification to each replicand:
//
coforall loc in Locales do on loc do
  RA = loc.id;

writeln("Replicated Array after assigning on each locale:");
writeReplicands(RA);
writeln();

//
// The following examples simply demonstrate that only the local
// replicand is accessed when an individual element is read...
//
on Locales[0] do
  writeln("on ", here, ": ", RA(Space.low));
on Locales[LocaleSpace.high] do
  writeln("on ", here, ": ", RA(Space.low));
writeln();
//
// ...or written:
//
on Locales[LocaleSpace.high] do
  RA(Space.low) = 7777;

writeln("Replicated Array after being indexed into");
writeReplicands(RA);
writeln();
//
// ...or the whole array is read:
//
on Locales[LocaleSpace.high] do
  A = RA + 4;
writeln("Non-Replicated Array after assignment from Replicated Array + 4");
writeln(A);
writeln();
