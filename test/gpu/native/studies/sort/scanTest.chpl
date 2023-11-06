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

var hillisScanArr = cpuArr;
on here.gpus[0]{
  var gpuArr = hillisScanArr;
  timer.start();
  hillisSteeleScan(gpuArr);
  timer.stop();
  hillisScanArr = gpuArr;
}
var gpuTime = timer.elapsed();
if printTimes then writeln("%30s".format("Hillis-Steele scan took : "), gpuTime);


var blellochScanArr = cpuArr;
on here.gpus[0]{
  var gpuArr = blellochScanArr;
  timer.clear();
  timer.start();
  blellochScan(gpuArr);
  timer.stop();
  blellochScanArr = gpuArr;
}
gpuTime = if timer.elapsed() > gpuTime then gpuTime else timer.elapsed();
if printTimes then writeln("%30s".format("Blelloch scan took : "), timer.elapsed());

var gpuScanArr = cpuArr;
on here.gpus[0]{
  var gpuArr = gpuScanArr;
  timer.clear();
  timer.start();
  gpuScan(gpuArr);
  timer.stop();
  gpuScanArr = gpuArr;
}
gpuTime = if timer.elapsed() > gpuTime then gpuTime else timer.elapsed();
if printTimes then writeln("%30s".format("GPU scan took : "), timer.elapsed());


timer.clear();
timer.start();
var a = + scan cpuArr;
timer.stop();
var cpuTime = timer.elapsed();
if printTimes then writeln("%30s".format("+ scan took : "), timer.elapsed());


timer.clear();
timer.start();
serialScan(cpuArr);
timer.stop();
cpuTime = if timer.elapsed() > cpuTime then cpuTime else timer.elapsed();
if printTimes then {
  writeln("%30s".format("CPU scan took : "), timer.elapsed());
  writeln("%30s".format("Ratio fastestGpu:fastestCpu : "), gpuTime/cpuTime);
}

// Check correctness
for i in cpuArr.domain {
  assert(hillisScanArr[i] == cpuArr[i]);
  assert(blellochScanArr[i] == cpuArr[i]);
  assert(gpuScanArr[i] == cpuArr[i]);
  // Since a has an inclusive scan and other are an exclusive scan:
  if i>a.domain.low then assert(a[i-1] == cpuArr[i]);
}
