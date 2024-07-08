use DistributedList;
use Collectives;

config const numTasks = 5,
             numAppendsPerTask = 5;

var b = new barrier(numTasks),
    dl = new distributedList(int);

coforall tid in 0..#numTasks with (ref dl) {
    for i in 1..#numAppendsPerTask {
        dl.append(i);
        b.barrier();
    }
}

// writeln(dl);
writeln(dl.size == numTasks * numAppendsPerTask);
writeln(+ reduce dl.toArray() == (numAppendsPerTask * (numAppendsPerTask + 1) / 2) * numTasks);
