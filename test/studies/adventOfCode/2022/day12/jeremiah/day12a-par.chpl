const (elevations, start, end) = readElevations();
const validMoves = findValidMoves(elevations);
writeln(findShortestPath(start, end, validMoves));

proc readElevations() {
    use IO;

    param a = "a".toByte(),
          S = "S".toByte(),
          E = "E".toByte();

    const elevBytes = stdin.lines().strip(),
          grid = {0..<elevBytes.size, 0..<elevBytes[0].size};

    var elevs = [(i, j) in grid] elevBytes[i][j].toByte() - a;

    const (_, start) = maxloc reduce zip((elevs == (S - a)), grid),
          (_, end)   = maxloc reduce zip((elevs == (E - a)), grid);

    elevs[start] = 0;
    elevs[end] = 25;

    return (elevs, start, end);
}

proc findValidMoves(elevs: [?d] uint(8)) {
    var moves: [d] uint(8);
    var elevs_signed : [d.expand(1)] int(8) = 27;
        elevs_signed[d] = elevs : int(8);

    forall idx in d do
        for (mask, dir) in ((1, (0, 1)), (2, (0, -1)), (4, (1, 0)), (8, (-1, 0))) do
            moves[idx] +=
                (if elevs_signed[idx] - elevs_signed[idx+dir] >= -1 then mask else 0): uint(8);


    return moves;
}

iter openMoves(m: uint(8)) {
    if m & 1 then yield (0, 1);
    if m & 2 then yield (0, -1);
    if m & 4 then yield (1, 0);
    if m & 8 then yield (-1, 0);
}

proc findShortestPath(start, end, moves: [?d] uint(8)) {
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
