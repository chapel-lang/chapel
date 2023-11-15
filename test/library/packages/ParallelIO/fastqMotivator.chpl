use ParallelIO, IO;
use fastQSequence;

config const nTasks = here.maxTaskPar;

// open a fastq file
var f = open("data.fastq", ioMode.r);

// read in parallel
var sequences = readParallelLocal(f, fastQSequence, nTasks);

// print a few sequences
const n = sequences.size;
writeln(n);
writeln(sequences[0].seq);
writeln(sequences[1].seq);
writeln(sequences[25].seq);
writeln(sequences[n-2].seq);
writeln(sequences[n-1].seq);
