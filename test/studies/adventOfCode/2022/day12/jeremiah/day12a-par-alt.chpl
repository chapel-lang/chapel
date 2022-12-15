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
    var minDistances: [d] atomic int = max(int);
    sync { explore(start, end, elevs, minDistances, 0); }
    return minDistances[end].read();
}

proc explore(
    pos: 2*int,
    end: 2*int,
    const ref elevs: [?d] int(8),
    ref minTo: [d] atomic int,
    pathLen: int
) {
    if pos == end then return;

    coforall nextPos in nextPositions(pos, end, elevs, minTo, pathLen + 1) do
        explore(nextPos, end, elevs, minTo, pathLen + 1);
}

iter nextPositions(pos, end, elevs, minTo, nextPathLen) {
    // stop searching if another path has reached 'end' in fewer steps
    if nextPathLen >= minTo[end].read() then return;

    // try moving in each direction
    label checkingMoves for move in ((1, 0), (-1, 0), (0, 1), (0, -1)) {
        const next = pos + move;

        // is this move on the map and valid?
        if elevs.domain.contains(next) &&
            elevs[pos] - elevs[next] >= -1 {

            // check if another path made it to 'next' in fewer steps
            //  if so, try the next direction
            //  otherwise, set minTo[next] = nextPathLen and yield
            var minToNext = minTo[next].read();
            do {
                if nextPathLen >= minToNext then continue checkingMoves;
            } while !minTo[next].compareExchange(minToNext, nextPathLen);

            yield next;
        }
    }
}

const (start, end, elevations) = readElevations();
writeln(findShortestPath(start, end, elevations));
