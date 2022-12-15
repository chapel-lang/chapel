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

    return (start, end, elevs:int(8));
}

proc findShortestPath(start, end, elevs: [?d] int(8)) {
    var minDistances: [d] int = max(int);
    explore(start, end, elevs, minDistances, 0);
    return minDistances[end];
}

proc explore(
    pos: 2*int,
    end: 2*int,
    const ref elevs: [?d] int(8),
    ref minTo: [d] int,
    pathLen: int
) {
    if pos == end then return;

    for nextPos in nextPositions(pos, end, elevs, minTo, pathLen + 1) do
        explore(nextPos, end, elevs, minTo, pathLen + 1);
}

iter nextPositions(pos, end, const ref elevs, ref minTo, nextPathLen) {
    // stop searching if another path has reached 'end' in fewer steps
    if nextPathLen >= minTo[end] then return;

    // try moving in each direction
    for move in ((1, 0), (-1, 0), (0, 1), (0, -1)) {
        const next = pos + move;

        // is this move on the map and valid?
        if elevs.domain.contains(next) &&
            elevs[pos] - elevs[next] >= -1 {

            // does this path beat the shortest record to 'next'?
            if nextPathLen < minTo[next] {
                minTo[next] = nextPathLen;
                yield next;
            }
        }
    }
}

const (start, end, elevations) = readElevations();
writeln(findShortestPath(start, end, elevations));
