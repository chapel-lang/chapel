use Time;

config const numTasks = 2;
config const numIncsPerTask = 1000;
var t: Timer;
var count$: sync int = 0;

t.clear();
t.start();

coforall j in 1..numTasks do
  for i in 1..numIncsPerTask do
    count$.writeEF(count$.readFE() + 1);

t.stop();

writeln(numIncsPerTask * numTasks, " sync var increments ",
        "in ", t.elapsed(), " secs");
writeln((t.elapsed() / (numIncsPerTask * numTasks)) * 1e9, " ns ",
        "per sync var increment");
