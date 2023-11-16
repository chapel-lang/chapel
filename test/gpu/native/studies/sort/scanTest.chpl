use GPU;
import Random;
import Time;
import IO.FormattedIO.format;

config const arrSize = 1<<11;
config const printTimes = false;
config const low = 0;
config const nonGpuTest = false;
config param multiDimArray = false;

if nonGpuTest {
  var cpuArr: [low..#arrSize] int;
  gpuScan(cpuArr);
}

// This code block is removed if multiDimArray is false
// So it doesn't cause a compilation error in that case
if multiDimArray {
  var cpuArr: [low..#arrSize, low..#arrSize] int;
  on here.gpus[0]{
    var gpuArr = cpuArr;
    gpuScan(gpuArr);
  }
}

var timer: Time.stopwatch;

testType(uint);
testType(int);
testType(real);

proc testType(type t) {
  var cpuArr: [low..#arrSize] t;
  Random.fillRandom(cpuArr);

  var gpuScanArr = cpuArr;
  on here.gpus[0]{
    var gpuArr = gpuScanArr;
    timer.clear();
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
    if i==a.domain.low then continue;

    if a.eltType == real then assert(isClose(a[i-1],gpuScanArr[i]));
    else assert(a[i-1] == gpuScanArr[i]);
  }
}
