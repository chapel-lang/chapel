param a = b"a".toByte(),
      S = b"S".toByte(),
      E = b"E".toByte();

const (elevations, start, end) = readElevations();
const validMoves = findValidMoves(elevations);
writeln(findShortestPath(start, end, validMoves, elevations));

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

    forall (i, j) in d do
       for (m, di, dj) in ((1, 0, 1), (2, 0, -1), (4, 1, 0), (8, -1, 0)) do
            moves[i, j] +=
                (if elevs[i, j] - _elevs[i+di, j+dj] >= -1 then m else 0) : uint(8);

    return moves;
}

iter openMoves(m: uint(8)) {
    if m & 1 then yield (0, 1);
    if m & 2 then yield (0, -1);
    if m & 4 then yield (1, 0);
    if m & 8 then yield (-1, 0);
}

proc findShortestPath(start, end, moves: [?d] uint(8), elevs: [d] int(8)): int {
    var minDistances: [d] int = max(int);
    var shortestA = max(int);
    explore(start, end, moves, elevs, minDistances, shortestA, 0);
    return shortestA;
}

proc explore(
    pos: 2*int, end: 2*int,
    const ref moves: [?d] uint(8),
    const ref elevs: [d] int(8),
    ref minDistances: [d] int,
    ref shortestSoFar: int,
    pathLen: int
) {
    // exit early if this path has already exceeded the record for shortest to 'E'
    if pathLen > shortestSoFar then return;

    // check if this path has beat the record for shortest to 'pos'
    if pathLen < minDistances[pos] {
        if elevs[pos] == 0
            then minDistances[pos] = 0;
            else minDistances[pos] = pathLen;

        // check if this path has reached 'E' and update record if needed
        if pos == end {
            if pathLen < shortestSoFar then shortestSoFar = pathLen;
            return;
        }

        // otherwise explore all the open moves starting from 'pos'
        for move in openMoves(moves[pos]) {
            explore(
                move + pos, end, moves, elevs, minDistances, shortestSoFar,
                if elevs[pos] == 0 then 1 else pathLen + 1
            );
        }
    }
}
