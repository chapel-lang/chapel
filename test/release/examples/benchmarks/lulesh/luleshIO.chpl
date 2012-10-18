config const filename = "lmeshes/sedov15oct.lmesh",  // input filename
             debugInit = false;      // print things as they're being init'ed

/* Initialization reads input variables from 'filename' */

var infile = open(filename, iomode.r);  // open the file
var reader = infile.reader();           // open a reader channel to the file


/* Read problem size */

proc getProblemSize() {
  const (numElems, numNodes) = reader.read(int, int);
  
  if debugInit then
    writeln("Using ", numElems, " elements, and ", numNodes, " nodes");

  return (numElems, numNodes);
}


/* Read input coordinates */

proc InitializeCoordinates(x,y,z) {
  for (locX,locY,locZ) in zip(x,y,z) do reader.read(locX, locY, locZ);

  if debugInit {
    writeln("locations are:");
    for (locX,locY,locZ) in zip(x,y,z) do
      writeln((locX, locY, locZ));
  }
}


/* Read element to node mapping */

proc initElemToNodeMapping(elemToNode: [?D]) {
  param nodesPerElem = elemToNode[D.low].size;
  for nodelist in elemToNode do 
    for i in 1..nodesPerElem do
      reader.read(nodelist[i]);

  if debugInit {
    writeln("elemToNode mappings are:");
    for nodelist in elemToNode do
      writeln(nodelist);
  }
}


/* read greek variables */

proc initGreekVars(lxim, lxip, letam, letap, lzetam, lzetap) {
  for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
    reader.read(xm,xp,em,ep,zm,zp);

  if debugInit {
    writeln("greek stuff:");
    for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
      writeln((xm,xp,em,ep,zm,zp));
  }
}



/* read symmetric boundaries */
                                           
inline proc initXSyms() {
  return readNodeset(reader, "XSym");
}                                             

inline proc initYSyms() {
  return readNodeset(reader, "YSym");
}                                             

inline proc initZSyms() {
  return readNodeset(reader, "ZSym");
}                                             


/* read free surface */

inline proc setupFreeSurface() {
  return readNodeset(reader, "freeSurface");
}


/* assert that we are at the end of the file (to make sure we didn't
   fail to read something */

proc finalizeInitialization() {
  reader.assertEOF("Input file format error (extra data at EOF)");
}


/* This is a helper routine to read a size and array from a file and
   return it */

proc readNodeset(reader, lbl) {
  const arrSize = reader.read(int);
  var A: [0..#arrSize] int;  // we may want to make this 'index(Nodes)'

  for a in A do
    reader.read(a);

  if debugInit then writeln(lbl, ": ", A);

  return (arrSize, A);
}
