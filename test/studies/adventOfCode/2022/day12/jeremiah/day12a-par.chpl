param a = b"a".toByte(),
      S = b"S".toByte(),
      E = b"E".toByte();

const (elevations, start, end) = readElevations();
const validMoves = findValidMoves(elevations);
writeln(findShortestPath(start, end, validMoves));

proc readElevations() {
    use IO;

    const elevBytes = stdin.readAll(bytes),
          w = elevBytes.find(b"\n"),
          h = elevBytes.count(b"\n");

    var elevs : [0..<h, 0..<w] int(8),
        start, end = (0, 0);

    for (row, i) in zip(elevBytes.split(b"\n"), 0..) {
        foreach (byte, j) in zip(row, 0..) {
            select byte {
                when S do start = (i, j);
                when E do end = (i, j);
                otherwise do elevs[i, j] = (byte - a) : int(8);
            }
        }
    }

    elevs[start] = 0;
    elevs[end] = 25;

    return (elevs, start, end);
}

proc findValidMoves(elevs: [?d] int(8)) : [d] uint(8) {
    var moves: [d] uint(8);
    var _elevs : [d.expand(1)] int(8) = 28;
    _elevs[d] = elevs;

    forall idx in d do
        for (mask, dir) in ( (1, (0, 1)), (2, (0, -1)), (4, (1, 0)), (8, (-1, 0)) ) do
            moves[idx] +=
                (if elevs[idx] - _elevs[idx+dir] >= -1 then mask else 0) : uint(8);

    return moves;
}

iter openMoves(m: uint(8)) {
    if m & 1 then yield (0, 1);
    if m & 2 then yield (0, -1);
    if m & 4 then yield (1, 0);
    if m & 8 then yield (-1, 0);
}

proc findShortestPath(start, end, moves: [?d] uint(8)): int {
    var minDistances: [d] atomic int = max(int);
    var shortestSoFar : atomic int = max(int);
    sync { explore(start, end, moves, minDistances, shortestSoFar, 0); }
    return minDistances[end].read();
}

proc explore(
    pos: 2*int, end: 2*int,
    const ref moves: [?d] uint(8),
    ref minDistances: [d] atomic int,
    ref shortestSoFar: atomic int,
    pathLen: int
) {
    // exit early if this path has already exceeded the record for shortest to 'E'
    if pathLen > shortestSoFar.read() then return;

    // check if this path has exceeded the record for shortest to 'pos'
    var currMin = minDistances[pos].read();
    do {
        if pathLen >= currMin then return;
    } while !minDistances[pos].compareExchange(currMin, pathLen);

    // check if this path has reached 'E' and update record if needed
    if pos == end {
        var currRecord = shortestSoFar.read();
        do {
            if pathLen >= currRecord then return;
        } while !shortestSoFar.compareExchange(currRecord, pathLen);
        return;
    }

    // otherwise explore all the open moves starting from 'pos' (spawning new tasks)
    for move in openMoves(moves[pos]) {
        const next = move + pos;
        begin explore(next, end, moves, minDistances, shortestSoFar, pathLen + 1);
    }
}
