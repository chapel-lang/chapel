use ParallelIO, IO;
use fastqSequence;

config const nTasks = here.maxTaskPar;

// read in parallel
var sequences = readParallelLocal("data.fastq", fastQSequence, nTasks);

// print a few sequences
const n = sequences.size;
writeln(n);
writeln(sequences[0].seq);
writeln(sequences[1].seq);
writeln(sequences[25].seq);
writeln(sequences[n-2].seq);
writeln(sequences[n-1].seq);
