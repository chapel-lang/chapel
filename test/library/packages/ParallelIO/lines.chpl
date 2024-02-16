use ParallelIO, IO, Random, Time, FileSystem;

const fileName = "test.txt";

config const nLines = 1000,
             strLen = 15,
             nTasks = 4;

var rng = new randomStream(uint(8));

config param CollectPerfData = false;

proc main() {
    const lines = makeRandLines(fileName, nLines);

    var s = new stopwatch();

    if CollectPerfData {
        // warmup
        serialReadLines(fileName);
        s.start();
        serialReadLines(fileName);
        writeln(s.elapsed());
    }

    if CollectPerfData then s.restart();
    const l1 = readLinesAsBlockArray(fileName, lineType=bytes, nTasks=nTasks);
    if CollectPerfData then writeln(s.elapsed());

    if CollectPerfData then s.restart();
    const l2 = readLinesAsArray(fileName, lineType=bytes, nTasks=nTasks);
    if CollectPerfData then writeln(s.elapsed());

    assert(l1.size == lines.size);
    assert(l2.size == lines.size);
    for ltest  in (l1, l2) {
        for (x, y, i) in zip(ltest, lines, 0..) {
            if x != y {
                writeln("mismatch at ", i, ": [", x, "] != [", y, "]");
                break;
            }
        }
    }

    remove(fileName);
}

proc makeRandLines(path: string, n: int): [] bytes throws {
    var f = open(path, ioMode.cwr),
        w = f.writer(locking=false),
        lines: [0..<n] bytes;

    for i in 0..<n {
        const line = randBytes(strLen);
        w.writeln(line);
        lines[i] = line;
    }

    return lines;
}

proc randBytes(len: int): bytes {
    use CTypes;
    const x = [i in 0..<len] rng.next(65, 122);
    return bytes.createCopyingBuffer(c_ptrToConst(x), len);
}

proc serialReadLines(path: string): [] bytes throws {
    use List;

    var f = open(path, ioMode.r),
        r = f.reader(locking=false),
        l = new list(bytes);

    var line: bytes;
    while r.readLine(line) do l.pushBack(line);

    return l.toArray();
}
