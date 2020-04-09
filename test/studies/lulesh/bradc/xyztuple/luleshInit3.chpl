private use IO;

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

param numDims = 3;

const nodesPerEdge = elemsPerEdge + 1,
      DimNodeRange = 0..#nodesPerEdge,
      DimNodeFace  = {DimNodeRange, DimNodeRange};



// read/compute the problem size

proc initProblemSize() {
  const (numElems, numNodes) = if (initFromFile) then
                                 reader.read(int, int)
                               else
                                 (elemsPerEdge**numDims, nodesPerEdge**numDims);

  if (debugInit) then
    writeln(numElems, " ", numNodes);

  return (numElems, numNodes); 
}


// read/compute the coordinates

proc initCoordinates(XYZ) {
  if (initFromFile) {
    for (x,y,z) in XYZ do
      reader.read(x, y, z);
  } else {
    forall (num, (x, y, z)) in zip(XYZ.domain, XYZ) {
      const (i,j,k) = nodeIdx1DTo3D(num);
      x = initialWidth * k / elemsPerEdge;
      y = initialWidth * j / elemsPerEdge;
      z = initialWidth * i / elemsPerEdge;
    }
  }

  if debugInit then
    for (x,y,z) in zip(XYZ) do
      writef("%.6dr %.6dr %.6dr\n", x, y, z);
}


// read/compute the element-to-node mapping

proc initElemToNodeMapping(elemToNode: [?D]) {
  if (initFromFile) {
    param nodesPerElem = elemToNode[D.low].size;
    for nodelist in elemToNode do 
      for i in 0..nodesPerElem-1 do
        reader.read(nodelist[i]);
    
    if debugInit {
      writeln("elemToNode mappings are:");
      for nodelist in elemToNode do
        writeln(nodelist);
    }
  } else {
    forall (num, nodelist) in zip(elemToNode.domain, elemToNode) {
      const (i,j,k) = elemIdx1DTo3D(num);
      nodelist[0] = nodeIdx3DTo1D(i,   j,   k  );
      nodelist[1] = nodeIdx3DTo1D(i,   j,   k+1);
      nodelist[2] = nodeIdx3DTo1D(i,   j+1, k+1);
      nodelist[3] = nodeIdx3DTo1D(i,   j+1, k  );
      nodelist[4] = nodeIdx3DTo1D(i+1, j,   k  );
      nodelist[5] = nodeIdx3DTo1D(i+1, j,   k+1);
      nodelist[6] = nodeIdx3DTo1D(i+1, j+1, k+1);
      nodelist[7] = nodeIdx3DTo1D(i+1, j+1, k  );
    }
  }

  if debugInit then
    for nodelist in elemToNode {
      for i in 0..#nodelist.size do
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

param X = 2,
      Y = 1,
      Z = 0;

inline proc initSyms(ref Sym, dir) {
  if (initFromFile) {
    readNodeset(Sym);
  } else {
    for ij in DimNodeFace do
      Sym += nodeIdx2DTo1D(ij, dir, 0);
  }

  if debugInit {
    writeln(Sym.size);
    for n in Sym do
      writeln(n);
  }

  return (Sym.size, Sym);
}

inline proc initXSyms(ref XSym) {
  initSyms(XSym, X);
}

inline proc initYSyms(ref YSym) {
  initSyms(YSym, Y);
}

inline proc initZSyms(ref ZSym) {
  initSyms(ZSym, Z);
}


// read/compute the free surface

inline proc initFreeSurface(ref freeSurface) {
  if (initFromFile) {
    readNodeset(freeSurface);
    reader.assertEOF("Input file format error (extra data at EOF)");
  } else {
    for ij in DimNodeFace do
      freeSurface += nodeIdx2DTo1D(ij, X, nodesPerEdge-1);
                  
    for ij in DimNodeFace do
      freeSurface += nodeIdx2DTo1D(ij, Y, nodesPerEdge-1);

    for ij in DimNodeFace do
      freeSurface += nodeIdx2DTo1D(ij, Z, nodesPerEdge-1);
  }

  if debugInit {
    use Sort;

    const size = freeSurface.size;
    var sortedSurface: [0..#size] int;

    for (a,b) in zip(sortedSurface, freeSurface) do a = b;
    sort(sortedSurface);

    writeln(size);
    for b in sortedSurface do
      writeln(b);
  }
  return (freeSurface.size, freeSurface);
}


// Helper routines for reading from a file

/* This is a helper routine to read a size and array from a file and
   return it */

proc readNodeset(ref nodeset) {
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

/* Turn a 2D node index into a 3D index by inserting the value
   'newval' in dimension 'newdim' */

inline proc nodeIdx2DTo1D(ij, newdim, newval) {
  var ijk: 3*int;
  for i in 0..#numDims do
    ijk[i] = if (i==newdim) then newval else ij[i - (i>newdim):int];
  return nodeIdx3DTo1D((...ijk));
}
