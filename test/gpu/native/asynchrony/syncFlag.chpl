use Time;

extern proc printf(s...);

config const n = 100;
config const reps = 100;
config const timeThreshold = 0.0001;
config const expectInstantenousKernelReturn = true;

var t: stopwatch;

var Arr: [0..#n] real;
on here.gpus[0] {
  var DevArr: [0..#n] real;

  t.start();
  @assertOnGpu
  foreach a in DevArr {
    for i in 0..#reps do a += sqrt(i);
  }
  t.stop();

  Arr = DevArr;
}

writeln(Arr[3]);

if expectInstantenousKernelReturn then assert(t.elapsed() < timeThreshold);
                                  else assert(t.elapsed() > timeThreshold);

