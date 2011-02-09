use GPUDist, GPUConstDist, Time;

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
param BLOCKSIZEX = 16 : int;
param BLOCKSIZEY = 8 : int;
param UNROLLX = 8 : int;

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

  // Print a checksum
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
  // Print several rows of the computed data
  for y in 0..16 do {
    for x in 0..#volsize.x do {
      var addr = y * volsize.x + x;
      outfile.write(energy(addr), " ");
      //outfile.writeln(energy(addr));
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

  param gsizeX = VOLSIZEX / (BLOCKSIZEX * UNROLLX);
  var rowSizeX = BLOCKSIZEX * gsizeX;

  const gridspacing = 0.1 : real(32);

  var atoms : [0..atomcount*4-1] real(32);

  initatoms(atoms, atomcount, volsize, gridspacing);

  var volmemsz = volsize.x * volsize.y * volsize.z;
  const volmemsz_dom = [0..VOLSIZEY-1, 0..VOLSIZEX-1];
  const atom_dom = 0..MAXATOMS-1;
  const energy_dom = 0..volmemsz-1;

  const GPUBlockDist = new dmap(new GPUDist(rank=2, tbSizeX=BLOCKSIZEX, tbSizeY=BLOCKSIZEY, gridSizeX=4, gridSizeY=64));
  const GPUBlockDist_atom = new dmap(new GPUDist(rank=1));
  const GPUCCBlockDist = new dmap(new GPUConstDist(rank=1));

  const space : domain(2, int) dmapped GPUBlockDist = volmemsz_dom;
  const atomspace : domain(1, int) dmapped GPUCCBlockDist = 0..MAXATOMS-1;
  const energyspace : domain(1, int) dmapped GPUBlockDist_atom = energy_dom;

  var runatoms : int;

  var atominfo : [atomspace] float4;

  var energygrid : [energyspace] real(32);

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

    var rundom = 0..(runatoms-1);

if timing then timer.start();
    forall (xindex,yindex) in space do {

      var xindex2 = (getBlockID_x() * getBlockSize_x()) * UNROLLX + getThreadID_x();
      var outaddr = (((getGridSize_x() * getBlockSize_x())  * UNROLLX) * yindex + xindex2) : int;

      var coory = gridspacing * (yindex : real(32));
      var coorx = gridspacing * (xindex2 : real(32));

      var energyvalx1 = 0.0 : real(32);
      var energyvalx2 = 0.0 : real(32);
      var energyvalx3 = 0.0 : real(32);
      var energyvalx4 = 0.0 : real(32);
      var energyvalx5 = 0.0 : real(32);
      var energyvalx6 = 0.0 : real(32);
      var energyvalx7 = 0.0 : real(32);
      var energyvalx8 = 0.0 : real(32);

      var gridspacing_u = gridspacing * (BLOCKSIZEX : real(32));

      //for atomid in 0..(runatoms-1) {
      for atomid in rundom {
        var dy = coory - atominfo(atomid).y;
        var dyz2 = (dy * dy) + atominfo(atomid).z;

        var dx1 = coorx - atominfo(atomid).x;
        var dx2 = dx1 + gridspacing_u;
        var dx3 = dx2 + gridspacing_u;
        var dx4 = dx3 + gridspacing_u;
        var dx5 = dx4 + gridspacing_u;
        var dx6 = dx5 + gridspacing_u;
        var dx7 = dx6 + gridspacing_u;
        var dx8 = dx7 + gridspacing_u;

        energyvalx1 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx1*dx1 + dyz2));
        energyvalx2 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx2*dx2 + dyz2));
        energyvalx3 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx3*dx3 + dyz2));
        energyvalx4 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx4*dx4 + dyz2));
        energyvalx5 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx5*dx5 + dyz2));
        energyvalx6 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx6*dx6 + dyz2));
        energyvalx7 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx7*dx7 + dyz2));
        energyvalx8 += atominfo(atomid).w * (1.0 : real(32) / sqrt(dx8*dx8 + dyz2));
      }

      energygrid(outaddr) += energyvalx1;
      energygrid(outaddr+1*BLOCKSIZEX) += energyvalx2;
      energygrid(outaddr+2*BLOCKSIZEX) += energyvalx3;
      energygrid(outaddr+3*BLOCKSIZEX) += energyvalx4;
      energygrid(outaddr+4*BLOCKSIZEX) += energyvalx5;
      energygrid(outaddr+5*BLOCKSIZEX) += energyvalx6;
      energygrid(outaddr+6*BLOCKSIZEX) += energyvalx7;
      energygrid(outaddr+7*BLOCKSIZEX) += energyvalx8;
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
