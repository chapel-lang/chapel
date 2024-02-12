use ParallelIO, IO, FileSystem, Random;

const fileName = "ints.txt";

config const n = 1000,
             nTasks = 4;

proc main() {
    const sumActual = makeIntsFile(fileName, n);

    var sum = 0;
    forall line in readLines(fileName, nTasks=nTasks) with (+ reduce sum) do
        sum += line:int;

    // ensure the file was read correctly
    assert(sum == sumActual, "sums don't match");
    remove(fileName);
}

proc makeIntsFile(path: string, n: int): int {
    var sum = 0,
        rng = new randomStream(int),
        f = open(path, ioMode.cwr),
        w = f.writer(locking=false);

    for 0..<n {
        const x = rng.next(0, 100);
        sum += x;
        w.writeln(x);
    }
    return sum;
}
