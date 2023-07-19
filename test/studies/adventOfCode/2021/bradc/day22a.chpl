use IO, List;

config const debug = false;

var numcubes: int;

var val: string;
var xlo, xhi, ylo, yhi, zlo, zhi: int;
var xmin, ymin, zmin = max(int), xmax, ymax, zmax = min(int);

record Region {
  var off: bool;
  var coords: domain(3);  // TODO: would've liked to use forwarding here

  proc this(r: Region) {
    return coords[r.coords];
  }
  proc contains(r: Region) {
    return r.coords[coords] == r.coords;
  }

  proc dim(d: int) {
    return coords.dim[d];
  }
}

var allRegions = readLines();

iter readLines() {
  while readf("%s x=%i..%i,y=%i..%i,z=%i..%i", val, xlo, xhi, ylo, yhi, zlo, zhi) {
    if debug then
      writeln("Got: ", (val, xlo, xhi, ylo, yhi, zlo, zhi));
    yield new Region(val == "off", {xlo..xhi, ylo..yhi, zlo..zhi});
    xmin = min(xlo, xmin);
    xmax = max(xhi, xmax);
    ymin = min(ylo, ymin);
    ymax = max(yhi, ymax);
    zmin = min(zlo, zmin);
    zmax = max(zhi, zmax);
  }
}

if debug then
  for r in allRegions do
    writeln(r);

var disjointRegions: list(Region);

iter findDisjointRegions(r, startFrom=0): Region {
  if debug then
    writeln("Looking for disjoint regions for ", r);
  var allDisjoint = true;
  for i in startFrom..<disjointRegions.size {
    ref r2 = disjointRegions[i];
    if r2[r].isEmpty() {
      if debug then
        writeln("Disjoint w.r.t. ", r2);
    } else if r2.contains(r) {
      if debug then
        writeln("Occluded by ", r2);
      return;
    } else {
      if debug then
        writeln("Intersecting");
      for r3 in unOccludedSections(r2, r) do
        for r4 in findDisjointRegions(r3, startFrom=i) do
          yield r4;
      return;
    }
  }
  if allDisjoint then {
    if debug then
      writeln("All disjoint");
    yield r;
  }
}

for i in allRegions.domain by -1 {
  var newRegions = findDisjointRegions(allRegions[i]);
  if debug then
    writeln("newRegions = ", newRegions);
  for r in newRegions do
    disjointRegions.pushBack(r);
}
if debug {
  for r in disjointRegions {
    writeln(r.coords);
    writeln(" = ", r.coords.size);
  }
}


writeln(+ reduce for r in disjointRegions do if !r.off then r.coords.size);


iter unOccludedSections(foreground, background): Region {
  if debug then
    writeln("** calculating region diff between ", foreground, " and ", background);
  for dim0 in regionDiff(foreground, background, 0) {
    for dim1 in regionDiff(foreground, background, 1) {
      for dim2 in regionDiff(foreground, background, 2) {
        const section = {dim0, dim1, dim2};
        if section != foreground.coords {
          if debug then
            writeln("*** slicing ", {dim0, dim1, dim2});
          yield new Region(background.off, {dim0, dim1, dim2});
        }
      }
    }
  }
}

iter regionDiff(foreground, background, dim) {
  var inds = background.dim(dim)[background.dim(dim).low..foreground.dim(dim).low-1];
  if inds.size > 0 then
    yield inds;
  inds = background.dim(dim)[foreground.dim(dim).low..foreground.dim(dim).high];
  yield inds;
  inds = background.dim(dim)[foreground.dim(dim).high+1..background.dim(dim).high];
  if inds.size > 0 then
    yield inds;
}
