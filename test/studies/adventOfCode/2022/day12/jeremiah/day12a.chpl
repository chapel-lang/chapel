use IO;

param a : int(8) = b"a".toByte(),
      S : int(8) = b"S".toByte(),
      E : int(8) = b"E".toByte();

const (elevations, start, end) = readElevations();
const validMoves = findValidMoves(elevations);
writeln(findShortestPath(start, end, validMoves));

proc readElevations() {
    const elevBytes = stdin.lines().strip(),
          grid = {0..<elevBytes.size, 0..<elevBytes[0].size};

    var elevs = [(i, j) in grid] elevBytes[i][j].toByte():int(8) - a;

    const (_, start) = maxloc reduce zip((elevs == (S - a)), grid),
          (_, end) = maxloc reduce zip((elevs == (E - a)), grid);

    elevs[start] = 0;
    elevs[end] = 25;

    return (elevs, start, end);
}

proc findValidMoves(elevs: [?d] int(8)) {
    var moves: [d] uint(8);
    var _elevs : [d.expand(1)] int(8) = 28;
    _elevs[d] = elevs;

    forall idx in d do
        for (mask, dir) in ((1, (0, 1)), (2, (0, -1)), (4, (1, 0)), (8, (-1, 0))) do
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

proc findShortestPath(start, end, moves: [?d] uint(8)) {
    var minDistances: [d] int = max(int);
    var shortestSoFar = max(int);
    explore(start, end, moves, minDistances, shortestSoFar, 0);
    return minDistances[end];
}

proc explore(
    pos: 2*int, end: 2*int,
    const ref moves: [?d] uint(8),
    ref minDistances: [d] int,
    ref shortestSoFar: int,
    pathLen: int
) {
    // exit early if this path has already exceeded the record for shortest to 'E'
    if pathLen > shortestSoFar then return;

    // check if this path has beat the record for shortest to 'pos'
    if pathLen < minDistances[pos] {
        minDistances[pos] = pathLen; // set the new record

        // check if this path has reached 'E' and update record if needed
        if pos == end {
            if pathLen < shortestSoFar then shortestSoFar = pathLen;
            return;
        }

        // otherwise explore all the open moves starting from 'pos'
        for move in openMoves(moves[pos]) {
            explore(move + pos, end, moves, minDistances, shortestSoFar, pathLen + 1);
        }
    }
}
