use ParallelIO, IO;
use fastqSequence;

config const nTasks = here.maxTaskPar;

const sequences = readParallelLocal("data.fastq", fastQSequence, nTasks);

const n = sequences.size;
writeln("read ", n, " sequences");
for i in 0..<5 do writeln(sequences[i].seq);
writeln("...");
for i in n-5..<n do writeln(sequences[i].seq);
