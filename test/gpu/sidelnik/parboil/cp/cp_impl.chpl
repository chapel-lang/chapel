use GPUDist, Time;

/*
A benchmark for computing the coulombic potential at each grid point over one 
plane in a 3D grid in which point charges have been randomly distributed. This 
benchmark was adapted from the 'cionize' benchmark in VMD.

See also:
  http://www.ks.uiuc.edu/

  J. E. Stone, J. C. Phillips, P. L. Freddolino, D. J. Hardy, L. G.  Trabuco, 
  and K. Schulten. "Accelerating Molecular Modeling Applications with Graphics 
  Processors." Journal of Computational Chemistry 28:2618-2640, 2007.
*/

config var outfile = false;

param ATOMCOUNT = 40000;
param MAXATOMS = 4000 : int;
param VOLSIZEX  = 512 : int;
param VOLSIZEY  = 512 : int;
param BLOCKSIZEX = 16;
param BLOCKSIZEY = 8;

config var timing : bool = true;
var timer: Timer;

record voldim3i {
  var x : int;
  var y : int;
  var z : int;
}

_extern record float4 {
  var x, y, z, w : real(32);
}

proc initatoms(inout atoms, count, volsize, gridspacing)
{
  use Random;
  var randnums = new RandomStream(seed = 54321);
  var size : voldim3i;

  size.x = (gridspacing * volsize.x : real(32)) : int;
  size.y = (gridspacing * volsize.y : real(32)) : int;
  size.z = (gridspacing * volsize.z : real(32)) : int;

  for i in 0..#count do {
    var addr = i * 4;
    atoms(addr) = (randnums.getNext() * size.x) : real(32);
    atoms(addr+1) = (randnums.getNext() * size.y) : real(32);
    atoms(addr+2) = (randnums.getNext() * size.z) : real(32);
    atoms(addr+3) = ((randnums.getNext() * 2.0) - 1.0) : real(32);
  }
    writeln("atoms = ", atoms(0), " and ", atoms(1));
}

proc copyatoms(atominfo, inout atoms, arr_index, count, zplane)
{
  if (count > MAXATOMS) then
    halt("ERROR!");

  for i in 0..#count do {
    var step = i * 4;
    atominfo(i).x = atoms(step:int + arr_index:int);
    atominfo(i).y = atoms(step:int+1+arr_index:int);
    var dz = zplane - atoms(step:int+2+arr_index:int);
    atominfo(i).z = (dz * dz) : real(32);
    atominfo(i).w = atoms(step:int+3+arr_index:int);
  }
}

proc writeenergy(filename, energy, volsize)
{
  var outfile = new file(filename, FileAccessMode.write);
  outfile.open();

  outfile.writeln(volsize.x," ",volsize.y," ",volsize.z," ", ATOMCOUNT);

  /* Print a checksum */
  var sum = 0.0;
  for y in 0..#volsize.y do {
    for x in 0..#volsize.x do {
      if (isnan(energy(y*volsize.x+x))) then
        writeln("NAN! x = ", x, " y = ", y);
      var t = energy(y * volsize.x + x);
      t = max(-20.0, min(20.0, t)) : real(32);
      sum = sum + t;
    }
  }
  outfile.writeln(sum);
  /* Print several rows of the computed data */
  for y in 0..16 do {
    for x in 0..#volsize.x do {
      var addr = y * volsize.x + x;
      outfile.write(energy(addr), " ");
    }
    outfile.writeln();
  }
  outfile.close();
}

proc main()
{

  var volsize : voldim3i;

  var atomcount = ATOMCOUNT;

  volsize.x = VOLSIZEX;
  volsize.y = VOLSIZEY;
  volsize.z = 1;

  const gridspacing = 0.1 : real(32);

  var atoms : [0..atomcount*4-1] real(32);

  initatoms(atoms, atomcount, volsize, gridspacing);

  var volmemsz = volsize.x * volsize.y * volsize.z;
  const volmemsz_dom = [0..VOLSIZEY-1, 0..VOLSIZEX-1];
  const atom_dom = [0..MAXATOMS-1];
  const energy_dom = [0..volmemsz-1];

  const GPUBlockDist = new dmap(new GPUDist(rank=2, tbSizeX=BLOCKSIZEX, tbSizeY=BLOCKSIZEY));
  const GPUBlockDist_atom = new dmap(new GPUDist(rank=1));

  const space : domain(2) dmapped GPUBlockDist = volmemsz_dom;
  const atomspace : domain(1) dmapped GPUBlockDist_atom = atom_dom;
  const energyspace : domain(1) dmapped GPUBlockDist_atom = energy_dom;

  var gridSizeX = (VOLSIZEX + BLOCKSIZEX -1) / BLOCKSIZEX;
  var rowSizeX = BLOCKSIZEX * gridSizeX;

  var runatoms : int;

  var atominfo : [atomspace] float4;

  var energygrid : [energyspace] real(32);
  var energy : [energy_dom] real(32);

  // Initialize to 0
  for i in energy_dom do
    energygrid(i) = 0.0 : real(32);

  var iterations = 0;
  var totaltime = 0.0;
  for atomstart in 0..#atomcount by MAXATOMS:int {
    var atomsremaining = atomcount - atomstart;
    if (atomsremaining > MAXATOMS) then
      runatoms = MAXATOMS : int;
    else
      runatoms = atomsremaining;

    copyatoms(atominfo, atoms, 4*atomstart, runatoms, 0 * gridspacing);

    //var atomdom = [0..#runatoms];

if timing then timer.start();
    forall (xindex,yindex) in space do {
      var energyval = 0.0 : real(32);
      var coorx = (gridspacing * xindex : real(32));
      var coory = (gridspacing * yindex : real(32));
      var outaddr = rowSizeX * yindex + xindex;
      //for atomid in atomdom do {
      for atomid in 0..(runatoms-1) {
        var dx = (coorx - atominfo(atomid).x);
        var dy = (coory - atominfo(atomid).y);
        var r_1 = (1.0 : real(32) / sqrt(dx * dx + dy * dy + atominfo(atomid).z));
        energyval += (atominfo(atomid).w * r_1);
      }
      energygrid(outaddr) += energyval; // : real(32);
    }
if timing then timer.stop();
totaltime += timer.elapsed();
if timing then writeln("Finished in : ", timer.elapsed(), " seconds, or ", timer.elapsed()/60, " minutes");
if timing then timer.clear();
  iterations += 1;
  }

writeln("total iterations = ", iterations);
writeln("totaltime = ", totaltime);
  if (outfile == true) then {
    writeenergy("cp_checksum.out", energygrid, volsize); 
  }
}
