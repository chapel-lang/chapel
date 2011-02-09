//use GPUDist, GPUExplicitConstDist, Time;
use GPUDist, GPUConstDist, Time;

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

record kValues {
  var Kx : real(32);
  var Ky : real(32);
  var Kz : real(32);
  var RhoPhiR : real(32);
  var RhoPhiI : real(32);
}

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

  const numX_range = 0..numX:int-1;
  const GPUBlockDist = new dmap(new GPUDist(rank=1, tbSizeX=KERNEL_RHO_PHI_THREADS_PER_BLOCK));
  const GPUBlockDist2 = new dmap(new GPUDist(rank=1, tbSizeX=KERNEL_FH_THREADS_PER_BLOCK));
  //const GPUConstantMemDist = new dmap(new GPUExplicitConstDist(rank=1));
  const GPUConstantMemDist = new dmap(new GPUConstDist(rank=1));
  const gpu_space : domain(1, int) dmapped GPUBlockDist = 0..numK:int-1;
  const gpu_numX_space : domain(1, int) dmapped GPUBlockDist2 = numX_range;
  var dR : [gpu_space] real(32);
  var dI : [gpu_space] real(32);
  var phiR : [gpu_space] real(32);
  var phiI : [gpu_space] real(32);
  var kx : [gpu_space] real(32);
  var ky : [gpu_space] real(32);
  var kz : [gpu_space] real(32);
  var x : [gpu_numX_space] real(32);
  var y : [gpu_numX_space] real(32);
  var z : [gpu_numX_space] real(32);
  var outR : [gpu_numX_space] real(32);
  var outI : [gpu_numX_space] real(32);

  inputData(infile, numK, numX, kx, ky, kz, x, y, z, phiR, phiI, dR, dI);
  fclose(infile);

  forall i in gpu_numX_space do {
    outR(i) = 0.0 : real(32);
    outI(i) = 0.0 : real(32);
  }

  writeln(numX, " pixels in output; ", numK, " samples in trajectory; using ", numK,"  samples");

  var constmemspace : domain(1, int) dmapped GPUConstantMemDist = 0..511;

  var realRhoPhi : [gpu_space] real(32);
  var imagRhoPhi : [gpu_space] real(32);

  /* Pre-compute the values of rhoPhi on the GPU */
  var totaltime = 0.0;
if timing then timer.start();
  forall indexK in gpu_space do {
    const (rPhiR, rPhiI, rDR, rDI) = (phiR(indexK), phiI(indexK), dR(indexK), dI(indexK));
    realRhoPhi(indexK) = rPhiR * rDR + rPhiI * rDI;
    imagRhoPhi(indexK) = rPhiR * rDI - rPhiI * rDR;
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();

  const numK_range = 0..numK-1;
  var kVals : [numK_range] kValues;
  var constMem : [constmemspace] kValues;

  for k in numK_range {
    kVals(k).Kx = kx(k);
    kVals(k).Ky = ky(k);
    kVals(k).Kz = kz(k);
    kVals(k).RhoPhiR = realRhoPhi(k);
    kVals(k).RhoPhiI = imagRhoPhi(k);
  }

if timing then timer.start();


  constMem = kVals;
  var FHGrids = (numK + KERNEL_FH_K_ELEMS_PER_GRID - 1)/ KERNEL_FH_K_ELEMS_PER_GRID;
  for FHGrid in 0..#FHGrids do {
    var kGlobalIndex = FHGrid * KERNEL_FH_K_ELEMS_PER_GRID;
    
    /* Albert --- temp */
    var numElems = min(KERNEL_FH_K_ELEMS_PER_GRID, numK - kGlobalIndex);
    writeln("numelems = ", numElems, " or ", numK - kGlobalIndex);
    constMem = kVals(kGlobalIndex..(kGlobalIndex+numElems-1));

    //constmemspace = 0..numElems-1;

/*
    var kIndex_range : range;
    if (kGlobalIndex + KERNEL_FH_K_ELEMS_PER_GRID < numK) then
      kIndex_range = kGlobalIndex..kGlobalIndex+KERNEL_FH_K_ELEMS_PER_GRID-1;
    else
      kIndex_range = kGlobalIndex..numK/4 - 1;
*/


    forall xIndex in gpu_numX_space do {
      const (sX, sY, sZ) = (x(xIndex), y(xIndex), z(xIndex));
      var (sOutR, sOutI) = (outR(xIndex), outI(xIndex));

      var kIndex = 0;
      var iIndex = kGlobalIndex;

      var kCnt = numK - kGlobalIndex;
      if kCnt < KERNEL_FH_K_ELEMS_PER_GRID then {
        while (kIndex < (kCnt % 4) && (iIndex < numK)) {
          const exponentArg = (PIx2 * (constMem(kIndex).Kx * sX + constMem(kIndex).Ky * sY + constMem(kIndex).Kz * sZ)) : real(32);
          const (cosArg, sinArg) = (cos(exponentArg), sin(exponentArg));
          const (cRhoPhiR, cRhoPhiI) = (constMem(kIndex).RhoPhiR, constMem(kIndex).RhoPhiI);
          sOutR += cRhoPhiR * cosArg - cRhoPhiI * sinArg;
          sOutI += cRhoPhiI * cosArg + cRhoPhiR * sinArg;
          kIndex += 1;
          iIndex += 1;
        }
      }
  
      //kIndex = kGlobalIndex;
      //iIndex = kGlobalIndex;
      //for kIndex in kIndex_range do {
      while ((kIndex < KERNEL_FH_K_ELEMS_PER_GRID) && (iIndex < numK)) {

        const exponentArg = (PIx2 * (constMem(kIndex).Kx * sX + constMem(kIndex).Ky * sY + constMem(kIndex).Kz * sZ)) : real(32);
        const (cosArg, sinArg) = (cos(exponentArg), sin(exponentArg));
        const (cRhoPhiR, cRhoPhiI) = (constMem(kIndex).RhoPhiR, constMem(kIndex).RhoPhiI);
        sOutR += cRhoPhiR * cosArg - cRhoPhiI * sinArg;
        sOutI += cRhoPhiI * cosArg + cRhoPhiR * sinArg;

        var kIndex1 = kIndex + 1;
        const exponentArg1 = (PIx2 * (constMem(kIndex1).Kx * sX + constMem(kIndex1).Ky * sY + constMem(kIndex1).Kz * sZ)) : real(32);
        const (cosArg1, sinArg1) = (cos(exponentArg1), sin(exponentArg1));
        const (cRhoPhiR1, cRhoPhiI1) = (constMem(kIndex1).RhoPhiR, constMem(kIndex1).RhoPhiI);
        sOutR += cRhoPhiR1 * cosArg1 - cRhoPhiI1 * sinArg1;
        sOutI += cRhoPhiI1 * cosArg1 + cRhoPhiR1 * sinArg1;

        var kIndex2 = kIndex + 2;
        const exponentArg2 = (PIx2 * (constMem(kIndex2).Kx * sX + constMem(kIndex2).Ky * sY + constMem(kIndex2).Kz * sZ)) : real(32);
        const (cosArg2, sinArg2) = (cos(exponentArg2), sin(exponentArg2));
        const (cRhoPhiR2, cRhoPhiI2) = (constMem(kIndex2).RhoPhiR, constMem(kIndex2).RhoPhiI);
        sOutR += cRhoPhiR2 * cosArg2 - cRhoPhiI2 * sinArg2;
        sOutI += cRhoPhiI2 * cosArg2 + cRhoPhiR2 * sinArg2;

        var kIndex3 = kIndex + 3;
        const exponentArg3 = (PIx2 * (constMem(kIndex3).Kx * sX + constMem(kIndex3).Ky * sY + constMem(kIndex3).Kz * sZ)) : real(32);
        const (cosArg3, sinArg3) = (cos(exponentArg3), sin(exponentArg3));
        const (cRhoPhiR3, cRhoPhiI3) = (constMem(kIndex3).RhoPhiR, constMem(kIndex3).RhoPhiI);
        sOutR += cRhoPhiR3 * cosArg3 - cRhoPhiI3 * sinArg3;
        sOutI += cRhoPhiI3 * cosArg3 + cRhoPhiR3 * sinArg3;

        kIndex += 4;
        iIndex += 4;
      }
     
     (outR(xIndex), outI(xIndex)) = (sOutR, sOutI);
    }
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();
if timing then timer.clear();
    writeln("Finished");

if timing then writeln("total gpu time = ", totaltime);
  outfile = fopen(outfilename, "wb");
  outputData(outfile, numX, outR, outI);
  fclose(outfile);
}
