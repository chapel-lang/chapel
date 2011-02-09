use GPUExplicitDist, Time;

config var timing : bool = true;
var timer: Timer;

//config const filename = "32_32_32_dataset.bin";
config const filename = "64_64_64_dataset.bin";
//config const outfilename  = "32_32_32_dataset.out";
config const outfilename  = "64_64_64_dataset.out";
param KERNEL_RHO_PHI_THREADS_PER_BLOCK = 512;
param KERNEL_FH_K_ELEMS_PER_GRID = 512;
param KERNEL_FH_THREADS_PER_BLOCK = 256;
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

def outputData(outfile, in numX, in outR, in outI)
{
  var size = numBytes(int(32));
  var ret : int;
  writeln("outputing ", size, " per element, and ", numX, " elements = ", size * numX);
  ret = fwrite(numX, size, 1, outfile);
  writeln("wrote ", ret, " bytes");
  ret = fwrite(outR(0), size, numX, outfile);
  writeln("wrote ", ret, " bytes");
  ret = fwrite(outI(0), size, numX, outfile);
  writeln("wrote ", ret, " bytes");
}

def main() {
  var numX, numK : int;
  var infile, outfile: _file;
  var ret : int;
  infile = fopen(filename, "rb");
  ret = fread(numK, numBytes(int), 1, infile);
  writeln(ret);
  ret = fread(numX, numBytes(int), 1, infile);
  writeln("ret = ", ret, " numK = ", numK, " numX = ", numX);
  var kx : [0..#numK] real(32);
  var ky : [0..#numK] real(32);
  var kz : [0..#numK] real(32);
  var x : [0..#numX] real(32);
  var y : [0..#numX] real(32);
  var z : [0..#numX] real(32);
  var phiR : [0..#numK] real(32);
  var phiI : [0..#numK] real(32);
  var dR: [0..#numK] real(32);
  var dI : [0..#numK] real(32);
  inputData(infile, numK, numX, kx, ky, kz, x, y, z, phiR, phiI, dR, dI);

  writeln(numX, " pixels in output; ", numK, " samples in trajectory; using ", numK,"  samples");

  fclose(infile);


  const numK_range = 0..numK-1;
  const GPUBlockDist = new dmap(new GPUExplicitDist(rank=1, tbSizeX=KERNEL_RHO_PHI_THREADS_PER_BLOCK));
  const GPUBlockDist2 = new dmap(new GPUExplicitDist(rank=1, tbSizeX=KERNEL_FH_THREADS_PER_BLOCK));
  const gpu_space : domain(1, int(64)) dmapped GPUBlockDist = 0..numK:int(64)-1;
  

  var realRhoPhi_d : [gpu_space] real(32);
  var imagRhoPhi_d : [gpu_space] real(32);

  var phiR_d : [gpu_space] real(32);
  var phiI_d : [gpu_space] real(32);
  var dR_d : [gpu_space] real(32);
  var dI_d : [gpu_space] real(32);
  phiR_d = phiR;
  phiI_d = phiI;
  dR_d = dR;
  dI_d = dI;

  /* Pre-compute the values of rhoPhi on the GPU */
  var totaltime = 0.0;
if timing then timer.start();
  forall indexK in gpu_space do {
    const (rPhiR, rPhiI, rDR, rDI) = (phiR_d(indexK), phiI_d(indexK), dR_d(indexK), dI_d(indexK));
    realRhoPhi_d(indexK) = rPhiR * rDR + rPhiI * rDI;
    imagRhoPhi_d(indexK) = rPhiR * rDI - rPhiI * rDR;
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();

  const numX_range = 0..numX:int(64)-1;
  const gpu_numX_space : domain(1, int(64)) dmapped GPUBlockDist2 = numX_range;
  var outR_d : [gpu_numX_space] real(32);
  var outI_d : [gpu_numX_space] real(32);
  var outR : [numX_range] real(32);
  var outI : [numX_range] real(32);
  var kx_d : [gpu_space] real(32);
  var ky_d : [gpu_space] real(32);
  var kz_d : [gpu_space] real(32);
  var x_d : [gpu_numX_space] real(32);
  var y_d : [gpu_numX_space] real(32);
  var z_d : [gpu_numX_space] real(32);
  kx_d = kx;
  ky_d = ky;
  kz_d = kz;
  x_d = x;
  y_d = y;
  z_d = z;

if timing then timer.start();
  forall i in gpu_numX_space do {
    outR_d(i) = 0.0 : real(32);
    outI_d(i) = 0.0 : real(32);
  }

  var FHGrids = (numK + KERNEL_FH_K_ELEMS_PER_GRID - 1)/ KERNEL_FH_K_ELEMS_PER_GRID;
  for FHGrid in 0..#FHGrids do {
    var kGlobalIndex = FHGrid * KERNEL_FH_K_ELEMS_PER_GRID;
/*
    var kIndex_range : range;
    if (kGlobalIndex + KERNEL_FH_K_ELEMS_PER_GRID < numK) then
      kIndex_range = kGlobalIndex..kGlobalIndex+KERNEL_FH_K_ELEMS_PER_GRID-1;
    else
      kIndex_range = kGlobalIndex..numK - 1;
*/
    forall xIndex in gpu_numX_space do {
      const (sX, sY, sZ) = (x_d(xIndex), y_d(xIndex), z_d(xIndex));
      var (sOutR, sOutI) = (outR_d(xIndex), outI_d(xIndex));
      
  
      var kIndex = kGlobalIndex;
      var iIndex = kGlobalIndex;
      while ((kIndex < (kGlobalIndex + KERNEL_FH_K_ELEMS_PER_GRID)) && (iIndex < numK)) {
 

      //for kIndex in kIndex_range do {
        const exponentArg = PIx2:real(32) * (kx_d(kIndex) * sX + ky_d(kIndex) * sY + kz_d(kIndex) * sZ);
        const (cosArg, sinArg) = (cos(exponentArg), sin(exponentArg));
        const (cRhoPhiR, cRhoPhiI) = (realRhoPhi_d(kIndex), imagRhoPhi_d(kIndex));

        sOutR += cRhoPhiR * cosArg - cRhoPhiI * sinArg;
        sOutI += cRhoPhiI * cosArg + cRhoPhiR * sinArg;

        kIndex += 1;
        iIndex += 1;

      }
     
     (outR_d(xIndex), outI_d(xIndex)) = (sOutR, sOutI);
    }
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();
if timing then timer.clear();
    writeln("Finished");

  outR = outR_d;
  outI = outI_d;
if timing then writeln("total gpu time = ", totaltime);
  outfile = fopen(outfilename, "wb");
  outputData(outfile, numX, outR, outI);
  fclose(outfile);
}
