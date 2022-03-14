use Channel;
use Time;
config const n = 10000;
const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                             else here.maxTaskPar;
config const performanceTest = false;
var t : Timer;
t.start();
var myc1 = new channel(int);
var myc2 = new channel(int);
var myc3 = new channel(int);
var done = new channel(int);
sync {
  for tid in 0..#numTasksPerLocale {
    begin with (ref myc1, ref myc2, ref myc3, ref done) {
      while(true) {
        var x1, x2, x3, x4 : int;
        var case1 : SelectBaseClass = new shared SelectCase(x1, myc1, selectOperation.send, 4);
        var case2 : SelectBaseClass = new shared SelectCase(x2, myc2, selectOperation.send, 5);
        var case3 : SelectBaseClass = new shared SelectCase(x3, myc3, selectOperation.send, 6);
        var case4 : SelectBaseClass = new shared SelectCase(x4, done, selectOperation.recv, 3);
        var idx = selectProcess([case1, case2, case3, case4]);
        if idx == 3 then break;
      }
    }
  }
  coforall tid in 0..#numTasksPerLocale with (ref myc1, ref myc2, ref myc3) {
    test();
  }
  done.close();
}
t.stop();
var elapsed = t.elapsed();

if performanceTest then writeln("Time per operation : ", elapsed * 1000 / n, " ms");

proc test() {
  for i in 0..#(n / numTasksPerLocale) {
    var x1, x2, x3 : int;
    var case1 : SelectBaseClass = new shared SelectCase(x1, myc1, selectOperation.recv, 0);
    var case2 : SelectBaseClass = new shared SelectCase(x2, myc2, selectOperation.recv, 1);
    var case3 : SelectBaseClass = new shared SelectCase(x3, myc3, selectOperation.recv, 2);
    var idx = selectProcess([case1, case2, case3]);
  }
}

