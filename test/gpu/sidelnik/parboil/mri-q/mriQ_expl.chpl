/***************************************************************************
 *cr
 *cr            (C) Copyright 2007 The Board of Trustees of the
 *cr                        University of Illinois
 *cr                         All Rights Reserved
 *cr
 ***************************************************************************/

/* 
 * C code for creating the Q data structure for fast convolution-based 
 * Hessian multiplication for arbitrary k-space trajectories.
 *
 * Inputs:
 * kx - VECTOR of kx values, same length as ky and kz
 * ky - VECTOR of ky values, same length as kx and kz
 * kz - VECTOR of kz values, same length as kx and ky
 * x  - VECTOR of x values, same length as y and z
 * y  - VECTOR of y values, same length as x and z
 * z  - VECTOR of z values, same length as x and y
 * phi - VECTOR of the Fourier transform of the spatial basis 
 *      function, evaluated at [kx, ky, kz].  Same length as kx, ky, and kz.
 *
*/

use GPUExplicitDist, Time;

config var timing : bool = true;
var timer: Timer;

//config const filename = "32_32_32_dataset.bin";
config const filename = "64_64_64_dataset.bin";
//config const outfilename  = "32_32_32_dataset.out";
config const outfilename  = "64_64_64_dataset.out";
param KERNEL_PHI_MAG_THREADS_PER_BLOCK = 512;
param KERNEL_Q_THREADS_PER_BLOCK = 256;
param KERNEL_Q_K_ELEMS_PER_GRID = 1024;
param PIx2 = 6.2831853071795864769252867665590058;

_extern def fopen(name: string, mode: string): _file;
_extern def fread(inout data, size: int, n: int, f: _file): int;
_extern def fwrite(inout data, size: int, n: int, f: _file): int;
_extern def fclose(f: _file);

def inputData(infile, _numK, _numX, inout kx, out ky, out kz, out x, out y, out z, out phiR, out phiI, out dR, out dI)
{
  var ret : int(32);
  var size = numBytes(real(32));
  ret = fread(kx(0), size, _numK, infile);
  writeln(ret);
  ret = fread(ky(0), size, _numK, infile);
  writeln(ret);
  ret = fread(kz(0), size, _numK, infile);
  writeln(ret);
  ret = fread(x(0), size, _numX, infile);
  writeln(ret);
  ret = fread(y(0), size, _numX, infile);
  writeln(ret);
  ret = fread(z(0), size, _numX, infile);
  writeln(ret);
  ret = fread(phiR(0), size, _numK, infile);
  writeln(ret);
  ret = fread(phiI(0), size, _numK, infile);
  writeln(ret);
  ret = fread(dR(0), size, _numK, infile);
  writeln(ret);
  ret = fread(dI(0), size, _numK, infile);
  writeln(ret);
}

def outputData(outfile, in numX, in Qr, in Qi)
{
  var size = numBytes(int(32));
  var ret : int;
  writeln("outputing ", size, " per element, and ", numX, " elements = ", size * numX);
  ret = fwrite(numX, size, 1, outfile);
  writeln("wrote ", ret, " bytes");
  ret = fwrite(Qr(0), size, numX, outfile);
  writeln("wrote ", ret, " bytes");
  ret = fwrite(Qi(0), size, numX, outfile);
  writeln("wrote ", ret, " bytes");
}

def main() {
  var numX, numK : int;
  var original_numK, original_numX : int(64);
  var infile, outfile: _file;
  var ret : int;
  infile = fopen(filename, "rb");
  ret = fread(numK, numBytes(int), 1, infile);
  writeln(ret);
  ret = fread(numX, numBytes(int), 1, infile);
  writeln(ret);
  writeln("numK = ", numK, " numX = ", numX);
  var kx : [0..#numK] real(32);
  var ky : [0..#numK] real(32);
  var kz : [0..#numK] real(32);
  var x : [0..#numX] real(32);
  var y : [0..#numX] real(32);
  var z : [0..#numX] real(32);
  var phiR : [0..#numK] real(32);
  var phiI : [0..#numK] real(32);
  var phiMag : [0..#numK] real(32);
  var dR: [0..#numK] real(32);
  var dI : [0..#numK] real(32);
  inputData(infile, numK, numX, kx, ky, kz, x, y, z, phiR, phiI, dR, dI);

  original_numK = numK : int(64);
  original_numX = numX : int(64);
  writeln(numX, " pixels in output; ", original_numK, " samples in trajectory; using ", numK,"  samples");

  fclose(infile);


  const numK_range = [0..original_numK-1];
  const GPUBlockDist_PhiMag = new dmap(new GPUExplicitDist(rank=1, tbSizeX=KERNEL_PHI_MAG_THREADS_PER_BLOCK));
  const GPUBlockDist_Qkern = new dmap(new GPUExplicitDist(rank=1, tbSizeX=KERNEL_Q_THREADS_PER_BLOCK));
  const gpu_space : domain(1, int(64)) dmapped GPUBlockDist_PhiMag = numK_range;

  var realRhoPhi_d : [gpu_space] real(32);
  var imagRhoPhi_d : [gpu_space] real(32);

  var phiR_d : [gpu_space] real(32);
  var phiI_d : [gpu_space] real(32);
  var phiMag_d : [gpu_space] real(32);
  var dR_d : [gpu_space] real(32);
  var dI_d : [gpu_space] real(32);
  phiR_d = phiR;
  phiI_d = phiI;
  dR_d = dR;
  dI_d = dI;

  /* GPU section 1 (precompute PhiMag) */
  var totaltime = 0.0;
if timing then timer.start();
  forall indexK in gpu_space do {
    const (real_val, imag_val) = (phiR_d(indexK), phiI_d(indexK));
    phiMag_d(indexK) = real_val * real_val + imag_val * imag_val;
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();

  phiMag = phiMag_d;

  const numX_range = [0..original_numX-1];
  const gpu_numX_space : domain(1, int(64)) dmapped GPUBlockDist_Qkern = numX_range;
  var kx_d : [gpu_space] real(32);
  var ky_d : [gpu_space] real(32);
  var kz_d : [gpu_space] real(32);
  var x_d : [gpu_numX_space] real(32);
  var y_d : [gpu_numX_space] real(32);
  var z_d : [gpu_numX_space] real(32);

  var Qr : [numX_range] real(32);
  var Qi : [numX_range] real(32);
  var Qr_d : [gpu_numX_space] real(32);
  var Qi_d : [gpu_numX_space] real(32);
  kx_d = kx;
  ky_d = ky;
  kz_d = kz;
  x_d = x;
  y_d = y;
  z_d = z;

  writeln("Second kernel");
  forall i in gpu_numX_space do {
    Qr_d(i) = 0.0 : real(32);
    Qi_d(i) = 0.0 : real(32);
  }

  var QGrids = (numK + KERNEL_Q_K_ELEMS_PER_GRID - 1)/ KERNEL_Q_K_ELEMS_PER_GRID;
  for QGrid in 0..#QGrids do {
    var kIndex_range : range;
    var kGlobalIndex = QGrid * KERNEL_Q_K_ELEMS_PER_GRID;
    if (kGlobalIndex + KERNEL_Q_K_ELEMS_PER_GRID < numK) then {
      kIndex_range = kGlobalIndex..kGlobalIndex+KERNEL_Q_K_ELEMS_PER_GRID-1;
    }
    else {
      kIndex_range = kGlobalIndex..numK - 1;
    }

    writeln("Third kernel");
if timing then timer.start();
    forall xIndex in gpu_numX_space do {
      const (sX, sY, sZ) = (x_d(xIndex), y_d(xIndex), z_d(xIndex));
      var (sQr, sQi) = (Qr_d(xIndex), Qi_d(xIndex));
      
      for kIndex in kIndex_range do {
        const phiMag_scalar = phiMag_d(kIndex);
        const exponentArg = PIx2: real(32) * (kx_d(kIndex) * sX + ky_d(kIndex) * sY + kz_d(kIndex) * sZ);
        sQr += phiMag_scalar * cos(exponentArg);
        sQi += phiMag_scalar * sin(exponentArg);
      }
      (Qr_d(xIndex), Qi_d(xIndex)) = (sQr, sQi);
    }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();
if timing then timer.clear();

  }

  Qr = Qr_d;
  Qi = Qi_d;


writeln("blah");
  for i in 0..99 do
    writeln(Qr(i));

if timing then writeln("total gpu time = ", totaltime);
//writeln("Qr = ", Qr);

  outfile = fopen(outfilename, "wb");
  outputData(outfile, numX, Qr, Qi);
  fclose(outfile);
}
