use GPU;
import Random;
import Time;
import IO.FormattedIO.format;

config const arrSize = 1<<11;
config const printTimes = false;
config const low = 0;

var cpuArr: [low..#arrSize] uint;
Random.fillRandom(cpuArr);
var timer: Time.stopwatch;


var gpuScanArr = cpuArr;
on here.gpus[0]{
  var gpuArr = gpuScanArr;
  timer.start();
  gpuScan(gpuArr);
  timer.stop();
  gpuScanArr = gpuArr;
}
const gpuTime = timer.elapsed();
if printTimes then writeln("GPU scan took : ", gpuTime);


timer.clear();
timer.start();
var a = + scan cpuArr;
timer.stop();
var cpuTime = timer.elapsed();
if printTimes then {
  writeln("%16s".format("CPU scan took : "), timer.elapsed());
  writeln("%16s".format("Ratio gpu:cpu : "), gpuTime/cpuTime);
}

// Check correctness
forall i in a.domain {
  // Since + scan is an inclusive scan and gpuScan is an exclusive scan:
  if i>a.domain.low then assert(a[i-1] == gpuScanArr[i]);
}
