use ParallelIO, IO, Time;
use fastqSequence;

config const nTasks = here.maxTaskPar,
             fileName = "data.fastq";
config param comparePerf = false;

var s = new stopwatch();

if comparePerf then s.start();
const sequences = readParallelLocal(fileName, fastQSequence, nTasks);

if comparePerf {
  s.stop();
  writeln("parallel time: ", s.elapsed(), " (sec)");

  s.restart();
  const seqSerial = serialReadFastQ(fileName);
  s.stop();
  writeln("serial time: ", s.elapsed(), " (sec)");
}

const n = sequences.size;
writeln("read ", n, " sequences");
for i in 0..<5 do writeln(sequences[i].seq);
writeln("...");
for i in n-5..<n do writeln(sequences[i].seq);
