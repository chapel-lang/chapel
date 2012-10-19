param numDims = 3;

config const elemsPerEdge = 15;
const        nodesPerEdge = elemsPerEdge + 1;

config const debugInit = false;

const DimElemRange = 0..#elemsPerEdge,
      DimNodeRange = 0..#nodesPerEdge;

const DimElemSpace = {DimElemRange, DimElemRange, DimElemRange},
      DimNodeFace  = {DimNodeRange, DimNodeRange},
      DimNodeSpace = {DimNodeRange, DimNodeRange, DimNodeRange};

proc getProblemSize() {
  if (debugInit) then
    writeln(DimElemSpace.size, " ", DimNodeSpace.size);

  return (DimElemSpace.size, DimNodeSpace.size); 
}

config const initialWidth = 1.125;

inline proc idx3DTo1D((i,j,k), len) {
  return i*len*len + j*len + k;
}

inline proc nodeIdx3DTo1D(i,j,k) {
  return idx3DTo1D((i, j, k), nodesPerEdge);
}

inline proc elemIdx3DTo1D(i,j,k) {
  return idx3DTo1D((i,j,k), elemsPerEdge);
}

inline proc idx1DTo3D(ind, len) {
  return (ind/(len*len), (ind/len)%len, ind%len);
}

/*
inline proc idx1DTo3D(ind, len) {
  return (ind/(len*len), ind%(len*len)/len, ind%len);
}
*/

inline proc nodeIdx1DTo3D(ind) {
  return idx1DTo3D(ind, nodesPerEdge);
}

inline proc elemIdx1DTo3D(ind) {
  return idx1DTo3D(ind, elemsPerEdge);
}

proc testit {
  var problem = false;

  for i in 0..#(elemsPerEdge**3) {
    const j = elemIdx3DTo1D((...elemIdx1DTo3D(i)));
    if (i != j) {
      writeln("elemIdx1DTo3D(", i, ") = ", elemIdx1DTo3D(i));
      writeln("but the reverse gives: ", j);
      problem = true;
    }
  }

  for i in 0..#(nodesPerEdge**3) {
    const j = nodeIdx3DTo1D((...nodeIdx1DTo3D(i)));
    if (i != j) {
      writeln("nodeIdx1DTo3D(", i, ") = ", nodeIdx1DTo3D(i));
      writeln("but the reverse gives: ", j);
      problem = true;
    }
  }

  if problem then halt("my mapping functions aren't inverses");
}


  

proc InitializeCoordinates(X, Y, Z) {
  forall (num, x, y, z) in zip(X.domain, X, Y, Z) {
    const (i,j,k) = nodeIdx1DTo3D(num);
    x = initialWidth * k / elemsPerEdge;
    y = initialWidth * j / elemsPerEdge;
    z = initialWidth * i / elemsPerEdge;
  }
  if debugInit then
    for (locX,locY,locZ) in zip(X,Y,Z) do
      writeln(format("%.6f", locX), " ", format("%.6f", locY), " ", 
              format("%.6f", locZ));
}


proc initElemToNodeMapping(elemToNode) {
  forall (num, nodelist) in zip(elemToNode.domain, elemToNode) {
    const (i,j,k) = elemIdx1DTo3D(num);
    nodelist[1] = nodeIdx3DTo1D(i,   j,   k  );
    nodelist[2] = nodeIdx3DTo1D(i,   j,   k+1);
    nodelist[3] = nodeIdx3DTo1D(i,   j+1, k+1);
    nodelist[4] = nodeIdx3DTo1D(i,   j+1, k  );
    nodelist[5] = nodeIdx3DTo1D(i+1, j,   k  );
    nodelist[6] = nodeIdx3DTo1D(i+1, j,   k+1);
    nodelist[7] = nodeIdx3DTo1D(i+1, j+1, k+1);
    nodelist[8] = nodeIdx3DTo1D(i+1, j+1, k  );
  }
  if debugInit then
    for nodelist in elemToNode {
      for i in 1..nodelist.size do
        write(nodelist(i), " ");
      writeln();
    }
}

proc initGreekVars(lxim, lxip, letam, letap, lzetam, lzetap) {
  forall num in lxim.domain {
    const (i,j,k) = elemIdx1DTo3D(num);

    lxim[num] = if (k == 0) 
                  then num
                  else elemIdx3DTo1D(i,j,k-1);

    lxip[num] = if (k == elemsPerEdge-1)
                  then num
                  else elemIdx3DTo1D(i,j,k+1);
    
    letam[num] = if (j == 0) 
                  then num
                  else elemIdx3DTo1D(i,j-1,k);

    letap[num] = if (j == elemsPerEdge-1)
                  then num
                  else elemIdx3DTo1D(i,j+1,k);
    
    lzetam[num] = if (i == 0) 
                  then num
                  else elemIdx3DTo1D(i-1,j,k);

    lzetap[num] = if (i == elemsPerEdge-1)
                  then num
                  else elemIdx3DTo1D(i+1,j,k);
  }
  if (debugInit) then
    for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
      writeln(xm, " ", xp, " ", em, " ", ep, " ", zm, " ", zp);
}

inline proc initXSyms() {
  const size = DimNodeFace.size;
  var A: [0..#size] int;

  for ((i,j),ind) in zip(DimNodeFace, 0..) do
    A[ind] = nodeIdx3DTo1D(i,j,0);

  if debugInit {
    writeln(size);
    for a in A do
      writeln(a);
  }

  return (size, A);
}

inline proc initYSyms() {
  const size = DimNodeFace.size;
  var A: [0..#size] int;

  for ((i,j),ind) in zip(DimNodeFace, 0..) do
    A[ind] = nodeIdx3DTo1D(i,0,j);

  if debugInit {
    writeln(size);
    for a in A do
      writeln(a);
  }

  return (size, A);
}

inline proc initZSyms() {
  const size = DimNodeFace.size;
  var A: [0..#size] int;

  for ((i,j),ind) in zip(DimNodeFace, 0..) do
    A[ind] = nodeIdx3DTo1D(0,i,j);

  if debugInit {
    writeln(size);
    for a in A do
      writeln(a);
  }

  return (size, A);
}

inline proc setupFreeSurface() {
  const size = DimNodeFace.size + (DimNodeFace.size - nodesPerEdge) + 
                 (DimNodeFace.size - (nodesPerEdge + nodesPerEdge - 1));
  var A: [0..#size] int;

  var ind = 0;
  for (i,j) in DimNodeFace {
    A[ind] = nodeIdx3DTo1D(i,j,nodesPerEdge-1);
    ind += 1;
  }
                  
  for (i,j) in DimNodeFace.interior(0,-(nodesPerEdge-1)) {
    A[ind] = nodeIdx3DTo1D(i,nodesPerEdge-1,j);
    ind += 1;
  }

  for (i,j) in DimNodeFace.interior(-(nodesPerEdge-1),-(nodesPerEdge-1)) {
    A[ind] = nodeIdx3DTo1D(nodesPerEdge-1,i,j);
    ind += 1;
  }

  if (ind != size) then halt("Didn't compute enough free surfaces");

  use Sort;
  QuickSort(A);

  if debugInit {
    writeln(size);
    for a in A do
      writeln(a);
  }

  return (size, A);
}

proc finalizeInitialization() {
}
