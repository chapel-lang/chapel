use ParallelIO, IO, Time;
use fastqSequence;

config const nTasks = here.maxTaskPar,
             fileName = "data.fastq";
config param comparePerf = false,
             UseDelim = false;

var s = new stopwatch();

if comparePerf then s.start();
const sequences = doParallelRead(fileName, fastQSequence, nTasks);

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

proc doParallelRead(fileName, type t, nTasks: int): [] t
  where UseDelim == false
    do return readParallelLocal(fileName, t, nTasks);

proc doParallelRead(fileName, type t, nTasks: int): [] t
  where UseDelim == true
    do return readParallelDelimitedLocal(fileName, delim=b"@", t, delimInclusive=true, nTasks);
