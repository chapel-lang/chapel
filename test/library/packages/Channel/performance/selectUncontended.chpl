use Channel;
use Time;
config const n = 10000;
const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
config const performanceTest = false;
var cnt = n / numTasksPerLocale;
var t : Timer;
t.start();
coforall tid in 0..#numTasksPerLocale {
    test();
}
t.stop();
var elapsed = t.elapsed();

if performanceTest then writeln("Time per operation : ", elapsed * 1000 / n, " ms");

proc test() {
    var myc1 = new channel(int, 1);
    var myc2 = new channel(int, 1);
    myc1.send(0);
    for i in 0..#cnt {
        var x1 : int;
        var x2 : int;
        var case1 : SelectBaseClass = new shared SelectCase(x1, myc1, selectOperation.recv, 0);
        var case2 : SelectBaseClass = new shared SelectCase(x2, myc2, selectOperation.recv, 0);
        var arr = [case1, case2];
        var idx = selectProcess(arr);
        if idx == 0 then myc2.send(0);
        else myc1.send(0);
    }
}