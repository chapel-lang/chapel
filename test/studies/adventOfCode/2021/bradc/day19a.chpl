use IO, Map, Set, Sort;

param X = 0, Y = 1, Z = 2;

config const numOverlapping = 12,
             debug = false;

enum item {empty=0, scanner, beacon};
use item;

var Landscape: map(3*int, item);

iter readScanner() {
  var header: string;
  readLine(header);
  if debug then
    writeln(header);
  var x, y, z: int;
  try {  // TODO: 'try' seems like it shouldn't be necessary here for scanner 0, yet it is due to the commas... is that as intended?  (vs. returning false?)
    while readf("%i,%i,%i\n", x,y,z) {
      yield (x,y,z);
    }
  } catch {
  }
  readln();
  if debug then
    writeln("tail:", header);
}

iter readAllScanners() {
  while true {
    var scanner = readScanner();
    if debug then
      writeln(scanner.size, ": ", scanner);
    if scanner.size == 0 then
      return;
    yield scanner;
  }
}

var allScanners = readAllScanners();

// Initialize landscape with the results of scanner 0
updateLandscape((0,0,0), scanner);
for b in allScanners[0] do
  updateLandscape(b, beacon);

if debug {
  printLandscape();

  writeln(allScanners);
  writeln(allScanners.domain);
}

var scannersToProcess: set(int);
var foundLinkToZero: [allScanners.domain] bool;
var XForm: [allScanners.domain] [X..Z, X..Z] int;
for d in X..Z do XForm[0][d,d] = 1;
var Shift: [allScanners.domain] 3*int;
var ScannerLoc: [allScanners.domain] 3*int;
foundLinkToZero[0] = true;
scannersToProcess.add(0);
do {
  var nextScanners: set(int);
  for si in scannersToProcess {
    for sj in 0..<allScanners.size {
      if !foundLinkToZero[sj] {
        ref s0 = allScanners[si];
        ref s1 = allScanners[sj];
        if debug then
          writeln((s0,s1));
        label nextPair for i in 0..<s0.size {
          var dists = dist2(s0[i], s0);
          for j in 0..<s1.size {
            var dists2 = dist2(s1[j], s1);
            sort(dists);
            sort(dists2);
            if debug {
              writeln(dists);
              writeln(dists2);
            }
            const matches = countMatches(dists, dists2);
            /*
          if matches == dists.size && dists.size == dists2.size then
            continue;
  */
            if matches >= numOverlapping {
              writeln("Found ", countMatches(dists, dists2), " matches starting from position ", (i,j), " between scanners ", si, " and ", sj);
              // Point i and j are the same
              nextScanners.add(sj);
              foundLinkToZero[sj] = true;
              correlate(s0, s1, i, j, si, sj);
              break nextPair;
            }
          }
        }
      }
    }
  }
  scannersToProcess = nextScanners;
} while !scannersToProcess.isEmpty();
var numBeacons = 0;
for (k,v) in Landscape.items() do
  if v == beacon then
    numBeacons += 1;
writeln(numBeacons);

writeln(ScannerLoc);
var maxDist = 0;
for s1 in ScannerLoc do
  for s2 in ScannerLoc do
    maxDist = max(maxDist, manhattanDist(s1, s2));
writeln(maxDist);
                  
proc countMatches(d0, d1) {
  var i, j, matches = 0;
  while i<d0.size && j<d1.size do
    if d0[i] == d1[j] {
      matches += 1;
      i += 1;
      j += 1;
    } else if d0[i] < d1[j] {
      i += 1;
    } else {
      j += 1;
    }
  return matches;
}

proc correlate(s0, s1, o0, o1, from, to) {
  for i in s0.domain {
    if i != o0 {
      var dist0 = dist2(s0[o0], s0[i]);
      if allDistsDiffer(s0[o0], s0[i]) {
        for j in s1.domain {
          var dist1 = dist2(s1[o1], s1[j]);
          if dist0 == dist1 {
            if debug then
              writeln("Found matching pairs between ", s0[o0], " and ", s0[i], " compared to ", s1[o1], " and ", s1[j]);
            computeTransformMatrix(s0, o0, i, s1, o1, j, from, to);
            return;
          }
        }
      }
    }
  }
}

proc computeTransformMatrix(s0, o0, p0, s1, o1, p1, from, to) {
  writeln(s0[o0], " correlates to ", s1[o1]);
  var xform: [X..Z, X..Z] int;
  for dim0 in X..Z {
    var delt0 = s0[p0](dim0)-s0[o0](dim0);
    for dim1 in X..Z {
      var delt1 = s1[p1](dim1)-s1[o1](dim1);
      if abs(delt0) == abs(delt1) {
        if debug then
          writeln("Dim ", dim0, " correlates with Dim ", dim1);
        xform[dim0, dim1] = if delt0 == delt1 then 1 else -1;
      }
    }
  }
  if debug then
    writeln("xform:\n", xform);
  XForm[to] = XForm[from] * xform;
  var shift = XForm[from]*s0[o0] - XForm[to]*s1[o1];
  Shift[to] = Shift[from] + shift;
  if debug then
    writeln("XForm[from]:\n", XForm[from]);
  if debug then
    writeln("XForm =\n", XForm[to]);
  writeln("Scanner at = ", Shift[to]);
  ScannerLoc[to] = Shift[to];
  updateLandscape(Shift[to], scanner);

  for coord in s1 do {
    if debug then
      writeln(XForm[to]*coord + Shift[to]);
    updateLandscape(XForm[to]*coord + Shift[to], beacon);
  }
}

operator *(M1: [X..Z, X..Z] int, M2: [X..Z, X..Z] int) {
  if debug then
    writeln("In my mat-mat mult, M1 =\n", M1, "M2 =\n", M2);
  var res: [X..Z, X..Z] int;
  for i in X..Z do
    for k in X..Z do
      res[i,k] = + reduce for j in X..Z do M1[i,j] * M2[j, k];
  if debug then
    writeln("res =\n", res);
  return res;
}

operator *(M: [X..Z, X..Z] int, v: 3*int) {
  var res: 3*int;
  for i in X..Z do
    res(i) = + reduce [j in X..Z] (M[i,j] * v(j));
  return res;
}


proc dist2(c0: 3*int, c1: 3*int) {
  return (c0(X)-c1(X))**2 + (c0(Y)-c1(Y))**2 + (c0(Z)-c1(Z))**2;
}

proc allDistsDiffer(c0: 3*int, c1: 3*int) {
  var x = abs(c0(X)-c1(X));
  var y = abs(c0(Y)-c1(Y));
  var z = abs(c0(Z)-c1(Z));
  return x != y && y != z && x != z;
}

proc updateLandscape(coord, kind) {
  if !Landscape.contains(coord) then
    Landscape.add(coord, kind);
  else if Landscape.getValue(coord) != kind then
    writeln("Attempted to store ", kind, " at ", coord, " but it's already full");
}

proc printLandscape() {
  writeln(Landscape);
}

proc manhattanDist(c0, c1) {
  return abs(c0[X]-c1[X]) + abs(c0[Y]-c1[Y]) + abs(c0[Z]-c1[Z]);
}
