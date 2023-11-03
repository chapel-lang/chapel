use GPU;
import Random;
import Time;

config const arrSize = 1<<10;
config const printTimes = false;


var cpuArr: [0..<arrSize] uint;
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
if printTimes then writeln("Hillis-Steele scan took : ", gpuTime);


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
if printTimes then writeln("Blelloch scan took : ", timer.elapsed());

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
if printTimes then writeln("GPU scan took : ", timer.elapsed());


timer.clear();
timer.start();
var a = + scan cpuArr;
timer.stop();
var cpuTime = timer.elapsed();
if printTimes then writeln("+ scan took : ", timer.elapsed());


timer.clear();
timer.start();
serialScan(cpuArr);
timer.stop();
cpuTime = if timer.elapsed() > cpuTime then cpuTime else timer.elapsed();
if printTimes then {
  writeln("CPU scan took : ", timer.elapsed());
  writeln("Ratio fastestGpu:fastestCpu : ", gpuTime/cpuTime);
}

// Check correctness
for i in 0..<arrSize {
  assert(hillisScanArr[i] == cpuArr[i]);
}
