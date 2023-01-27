use DistributedList;
use Barriers;

config const numTasks = 10,
             numAppendsPerTask = 10;

var b = new Barrier(numTasks),
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
