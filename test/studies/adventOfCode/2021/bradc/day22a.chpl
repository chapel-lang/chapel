use IO, List;

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

proc addCube(newCube) {
  
}

var allRegions = readLines();

iter readLines() {
  while stdin.readf("%s x=%i..%i,y=%i..%i,z=%i..%i", val, xlo, xhi, ylo, yhi, zlo, zhi) {
//    writeln("Got: ", (val, xlo, xhi, ylo, yhi, zlo, zhi));
    yield new Region(val == "off", {xlo..xhi, ylo..yhi, zlo..zhi});
    xmin = min(xlo, xmin);
    xmax = max(xhi, xmax);
    ymin = min(ylo, ymin);
    ymax = max(yhi, ymax);
    zmin = min(zlo, zmin);
    zmax = max(zhi, zmax);
  }
}

//writeln(allRegions);
/*
for r in allRegions {
  writeln(r);
  forall i in r.coords do ;
}
*/

var disjointRegions: list(Region);
//disjointRegions.add(allRegions.last);

iter findDisjointRegions(r, startFrom=0): Region {
//  writeln("Looking for disjoint regions for ", r);
  var allDisjoint = true;
  for i in startFrom..<disjointRegions.size {
    ref r2 = disjointRegions[i];
    if r2[r].isEmpty() {
//      writeln("Disjoint w.r.t. ", r2);
    } else if r2.contains(r) {
//      writeln("Occluded by ", r2);
      return;
    } else {
//      writeln("Intersecting");
      for r3 in unOccludedSections(r2, r) do
        for r4 in findDisjointRegions(r3, startFrom=i) do
          yield r4;
      return;
    }
  }
  if allDisjoint then {
//    writeln("All disjoint");
    yield r;
  }
}

for i in allRegions.domain by -1 {
  var newRegions = findDisjointRegions(allRegions[i]);
//  writeln("newRegions = ", newRegions);
  for r in newRegions do
    disjointRegions.append(r);
}
//writeln(disjointRegions);

/*
for r in disjointRegions {
  writeln(r.coords);
  writeln(" = ", r.coords.size);
}
*/


// BUG: writeln(+ reduce [r in disjointRegions] if !r.off then r.coords.size);

// OK:
writeln(+ reduce for r in disjointRegions do if !r.off then r.coords.size);


iter unOccludedSections(foreground, background): Region {
//  writeln("** calculating region diff between ", foreground, " and ", background);
  for dim0 in regionDiff(foreground, background, 0) {
    for dim1 in regionDiff(foreground, background, 1) {
      for dim2 in regionDiff(foreground, background, 2) {
        const section = {dim0, dim1, dim2};
        if section != foreground.coords {
//          writeln("*** slicing ", {dim0, dim1, dim2});
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

/*
for i in allRegions.domain by -1 {
  var newDisjoints = addDisjointRegions(allRegions[i]);
}
*/

/*
const BBox = {xmin..xmax, ymin..ymax, zmin..zmax};
writeln(BBox);
forall i in BBox {
  if i(1)%10000 == 0 then
    writeln(i);
}

var onRegions: list(Region);
for r in allRegions {
  var newOnRegions: list(Region);

}
*/

/*
  const setting = val == "on";
  const SubD = ;
  if setting {
    var foundIntersection = false;
    for c in 1..numcubes do
      if !A[c][SubD].isEmpty() {
        writeln("Intersection between ", A[c], " and ", SubD);
        foundIntersection = true;
        if A[c].isSuper(SubD) then
          writeln("But it's subsumed");
        else {
          for newcube in union(A[c], SubD) do
            
        }
      }
    if !foundIntersection {
      writeln("No intersection");
      numcubes += 1;
      A[numcubes] = SubD;
    }
  } else {
*/
/*
    for corig in A {
      const c = corig;
      if c[SubD].size != 0 {
        writeln("Computing the intersections between ", c, " and ", SubD);
        /* clo slo shi chi
           slo clo shi chi
           slo clo chi shi
        */           
        var xs = [c.dim(0).low, SubD.dim(0).low, SubD.dim(0).high, c.dim(0).high];
        var ys = [c.dim(1).low, SubD.dim(1).low, SubD.dim(1).high, c.dim(1).high];
        var zs = [c.dim(2).low, SubD.dim(2).low, SubD.dim(2).high, c.dim(2).high];
        var first = true;
        for x in 0..<xs.size-1 {
          var xdim = xs[x]..xs[x+1];
          if xdim.size > 0 {
            for y in 0..<ys.size-1 {
              var ydim = ys[y]..ys[y+1];
              if ydim.size > 0 {
                for z in 0..<zs.size-1 {
                  var zdim = zs[z]..zs[z+1];
                  if zdim.size > 0 {
//                    writeln("Found one at: ", {xdim, ydim, zdim});
		    const IntD = c[{xdim, ydim, zdim}];
                    writeln("Found one at: ", IntD);
                    if (first) then corig = IntD; else {
                      numcubes += 1;
                      A[numcubes] = IntD;
                    }
                   
                  }            
                }
              }
            }
          }
        }
      }
    }
*/
/*
  }
}
*/


