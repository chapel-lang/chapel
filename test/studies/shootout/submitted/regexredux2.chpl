/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Ben Harshbarger
   derived from the GNU C++ RE2 version by Alexey Zolotov
*/

use IO, Regex;

proc main(args: [] string) {
  var variants = [
    "agggtaaa|tttaccct",
    "[cgt]gggtaaa|tttaccc[acg]",
    "a[act]ggtaaa|tttacc[agt]t",
    "ag[act]gtaaa|tttac[agt]ct",
    "agg[act]taaa|ttta[agt]cct",
    "aggg[acg]aaa|ttt[cgt]ccct",
    "agggt[cgt]aa|tt[acg]accct",
    "agggta[cgt]a|t[acg]taccct",
    "agggtaa[cgt]|[acg]ttaccct"
  ];

  var subst = [
    ("tHa[Nt]", "<4>"), ("aND|caN|Ha[DS]|WaS", "<3>"), ("a[NSt]|BY", "<2>"), 
    ("<[^>]*>", "|"), ("\\|[^|][^|]*\\|", "-")
  ];

  var data = stdin.readAll(string); // read in the entire file
  const initLen = data.size;

  // remove newlines
  data = data.replace(new regex(">.*\n|\n"), "");

  var copy = data; // make a copy so we can perform replacements in parallel

  var results: [variants.domain] int;

  sync {
    // fire off a task to perform replacements
    begin with (ref copy) {
      for (f, r) in subst do
        copy = copy.replace(new regex(f), r);
    }

    // count patterns
    forall (pattern, result) in zip(variants, results) do
      for m in (new regex(pattern)).matches(data) do
        result += 1;
  }

  // print results
  for (p,r) in zip(variants, results) do
    writeln(p, " ", r);
  writeln();

  writeln(initLen);
  writeln(data.size);
  writeln(copy.size);
}
