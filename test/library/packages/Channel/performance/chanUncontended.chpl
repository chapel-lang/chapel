use Channel;
use Time;
config const n = 10000;
const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
config const C = 100;
config const performanceTest = false;
var t : Timer;
t.start();

forall i in 0..#n {
    test();
}
t.stop();
var elapsed = t.elapsed();

if performanceTest then writeln("Time per operation : ", elapsed * 1000 / n, " ms");

proc test() {
    var myc = new channel(int, C);
    for i in 0..#C {
        myc.send(0);
    }
    for i in 0..#C {
        var x1 : int;
        myc.recv(x1);
    }
}
