use ParallelIO, IO;
use fastqSequence;

config const nTasksPerLoc = 1;

// read in parallel across 'Locales' (test expects "-nl4")
var sequences = readParallel("data.fastq", fastQSequence, nTasksPerLoc);

// print a few sequences from each locale
for param i in 0..<4 do on Locales[i] {
  writeln("Locale: ", i, " ------------------");
  const n = sequences[i].size;
  writeln(n, "\t", sequences[i].locale.id);
  writeln(sequences[i][0].seq);
  writeln(sequences[i][1].seq);
  writeln(sequences[i][5].seq);
  writeln(sequences[i][n-2].seq);
  writeln(sequences[i][n-1].seq);
  writeln("---------------------------");
}
