config const debugInit = false;


// Configs if we're init-ing from a file

config const filename = "";
const initFromFile = (filename != "");


// Configs if we're computing the input set

config const elemsPerEdge = 0;
config const initialWidth = 1.125;


// Verify that configs are set appropriately

if initFromFile {
  if elemsPerEdge != 0 then
    writeln("WARNING: --elemsPerEdge has no effect when using an input file");
}  else {
  if elemsPerEdge == 0 then
    halt("you must either set --filename or --elemsPerEdge to run LULESH");
}


// Helper variables for reading from a file

var infile: file;
var reader: channel(false, iokind.dynamic, true);

if initFromFile {
  infile = open(filename, iomode.r);
  reader = infile.reader();
}


// Helper variables for computing the index set
const        nodesPerEdge = elemsPerEdge + 1;

const DimNodeRange = 0..#nodesPerEdge,
      DimNodeFace  = {DimNodeRange, DimNodeRange};



// read/compute the problem size

proc getProblemSize() {
  param        numDims = 3;
  const (numElems, numNodes) = if (initFromFile) then
                                 reader.read(int, int)
                               else
                                 (elemsPerEdge**numDims, nodesPerEdge**numDims);

  if (debugInit) then
    writeln(numElems, " ", numNodes);

  return (numElems, numNodes); 
}


// read/compute the coordinates

proc InitializeCoordinates(X, Y, Z) {
  if (initFromFile) {
    for (x,y,z) in zip(X,Y,Z) do
      reader.read(x, y, z);
  } else {
    forall (num, x, y, z) in zip(X.domain, X, Y, Z) {
      const (i,j,k) = nodeIdx1DTo3D(num);
      x = initialWidth * k / elemsPerEdge;
      y = initialWidth * j / elemsPerEdge;
      z = initialWidth * i / elemsPerEdge;
    }
  }

  if debugInit then
    for (x,y,z) in zip(X,Y,Z) do
      writeln(format("%.6f",x), " ", format("%.6f",y), " ", format("%.6f",z));
}


// read/compute the element-to-node mapping

proc initElemToNodeMapping(elemToNode: [?D]) {
  if (initFromFile) {
    param nodesPerElem = elemToNode[D.low].size;
    for nodelist in elemToNode do 
      for i in 1..nodesPerElem do
        reader.read(nodelist[i]);
    
    if debugInit {
      writeln("elemToNode mappings are:");
      for nodelist in elemToNode do
        writeln(nodelist);
    }
  } else {
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
  }

  if debugInit then
    for nodelist in elemToNode {
      for i in 1..nodelist.size do
        write(nodelist(i), " ");
      writeln();
    }
}


// read/compute the greek variables

proc initGreekVars(lxim, lxip, letam, letap, lzetam, lzetap) {
  if (initFromFile) {
    for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
      reader.read(xm,xp,em,ep,zm,zp);
  } else {
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
  }
  if (debugInit) then
    for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
      writeln(xm, " ", xp, " ", em, " ", ep, " ", zm, " ", zp);
}


// read/compute the X, Y, Z symmetry planes

inline proc initXSyms(XSym) {
  if (initFromFile) {
    readNodeset(XSym);
  } else {
    for ((i,j),ind) in zip(DimNodeFace, 0..) do
      XSym += nodeIdx3DTo1D(i,j,0);
  }

  if debugInit {
    writeln(XSym.numIndices);
    for x in XSym do
      writeln(x);
  }

  return (XSym.numIndices, XSym);
}

inline proc initYSyms(YSym) {
  if (initFromFile) {
    readNodeset(YSym);
  } else {
    for ((i,j),ind) in zip(DimNodeFace, 0..) do
      YSym += nodeIdx3DTo1D(i,0,j);
  }

  if debugInit {
    writeln(YSym.numIndices);
    for x in YSym do
      writeln(x);
  }

  return (YSym.numIndices, YSym);
}

inline proc initZSyms(ZSym) {
  if (initFromFile) {
    readNodeset(ZSym);
  } else {
    for ((i,j),ind) in zip(DimNodeFace, 0..) do
      ZSym += nodeIdx3DTo1D(0,i,j);
  }

  if debugInit {
    writeln(ZSym.numIndices);
    for x in ZSym do
      writeln(x);
  }

  return (ZSym.numIndices, ZSym);
}


// read/compute the free surface

inline proc setupFreeSurface(freeSurface) {
  if (initFromFile) {
    readNodeset(freeSurface);
    reader.assertEOF("Input file format error (extra data at EOF)");
  } else {
    for (i,j) in DimNodeFace do
      freeSurface += nodeIdx3DTo1D(i,j,nodesPerEdge-1);
                  
    for (i,j) in DimNodeFace do
      freeSurface += nodeIdx3DTo1D(i,nodesPerEdge-1,j);

    for (i,j) in DimNodeFace do
      freeSurface += nodeIdx3DTo1D(nodesPerEdge-1,i,j);
  }

  if debugInit {
    use Sort;

    const size = freeSurface.size;
    var sortedSurface: [0..#size] int;

    for (a,b) in zip(sortedSurface, freeSurface) do a = b;
    QuickSort(sortedSurface);

    writeln(size);
    for b in sortedSurface do
      writeln(b);
  }
  return (freeSurface.numIndices, freeSurface);
}


// Helper routines for reading from a file

/* This is a helper routine to read a size and array from a file and
   return it */

proc readNodeset(nodeset) {
  const arrSize = reader.read(int);

  for a in 0..#arrSize do
    nodeset += reader.read(int);
}


// Helper routines for computing the cube input set

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

inline proc nodeIdx1DTo3D(ind) {
  return idx1DTo3D(ind, nodesPerEdge);
}

inline proc elemIdx1DTo3D(ind) {
  return idx1DTo3D(ind, elemsPerEdge);
}


