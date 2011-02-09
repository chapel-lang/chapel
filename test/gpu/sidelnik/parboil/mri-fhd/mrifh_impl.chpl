use GPUDist, Time;

config var timing : bool = true;
var timer: Timer;

//config const filename = "32_32_32_dataset.bin";
config const filename = "64_64_64_dataset.bin";
//config const outfilename  = "32_32_32_dataset.out";
config const outfilename  = "64_64_64_dataset.out";
param KERNEL_RHO_PHI_THREADS_PER_BLOCK = 512;
param KERNEL_FH_K_ELEMS_PER_GRID = 512;
param PIx2 = 6.2831853071795864769252867665590058;

_extern def fopen(name: string, mode: string): _file;
_extern def fread(inout data, size: int, n: int, f: _file): int;
_extern def fwrite(inout data, size: int, n: int, f: _file): int;
_extern def fclose(f: _file);

def inputData(infile, _numK, _numX ,out kx, out ky, out kz, out x, out y, out z, out phiR, out phiI, out dR, out dI)
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
//def outputData(outfile, numX, outR, outI)
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

  const GPUBlockDist = new dmap(new GPUDist(rank=1, tbSizeX=KERNEL_RHO_PHI_THREADS_PER_BLOCK));
  const gpu_space : domain(1) dmapped GPUBlockDist = 0..numK-1;
  const gpu_numX_space : domain(1) dmapped GPUBlockDist = 0..numX-1;
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

  var realRhoPhi : [gpu_space] real(32);
  var imagRhoPhi : [gpu_space] real(32);

  /* Pre-compute the values of rhoPhi on the GPU */
  var totaltime = 0.0;
if timing then timer.start();
/*
  forall indexK in gpu_space do {
    const (rPhiR, rPhiI, rDR, rDI) = (phiR(indexK), phiI(indexK), dR(indexK), dI(indexK));
    realRhoPhi(indexK) = rPhiR * rDR + rPhiI * rDI;
    imagRhoPhi(indexK) = rPhiR * rDI - rPhiI * rDR;
  }
  */
  forall (rPhiR, rPhiI, rDR, rDI, realRho, imagRho) in (phiR, phiI, dR, dI, realRhoPhi, imagRhoPhi) do {
    realRho = rPhiR * rDR + rPhiI * rDI;
    imagRho = rPhiR * rDI - rPhiI * rDR;
  }
if timing then timer.stop();
if timing then totaltime += timer.elapsed();

if timing then timer.start();
/*
  forall i in gpu_numX_space do {
    outR(i) = 0.0 : real(32);
    outI(i) = 0.0 : real(32);
  }
*/
  forall (outr, outi) in (outR, outI) do
    (outr, outi) = (0.0 : real(32) , 0.0 :real(32));

  var FHGrids = (numK + KERNEL_FH_K_ELEMS_PER_GRID - 1)/ KERNEL_FH_K_ELEMS_PER_GRID;
  for FHGrid in 0..#FHGrids do {
    var kGlobalIndex = FHGrid * KERNEL_FH_K_ELEMS_PER_GRID;

    var kIndex_range : range;
    if (kGlobalIndex + KERNEL_FH_K_ELEMS_PER_GRID < numK) then
      kIndex_range = kGlobalIndex..kGlobalIndex+KERNEL_FH_K_ELEMS_PER_GRID-1;
    else
      kIndex_range = kGlobalIndex..numK - 1;


    /*
    forall xIndex in gpu_numX_space do {
      const (sX, sY, sZ) = (x(xIndex), y(xIndex), z(xIndex));
      var (sOutR, sOutI) = (outR(xIndex), outI(xIndex));
     
      //var kIndex = kGlobalIndex;
      //var iIndex = kGlobalIndex;
      
      //while ((kIndex < (kGlobalIndex + KERNEL_FH_K_ELEMS_PER_GRID)) && (iIndex < numK)) do {
      for kIndex in kIndex_range do {
        const exponentArg = PIx2:real(32) * (kx(kIndex) * sX + ky(kIndex) * sY + kz(kIndex) * sZ);
        const (cosArg, sinArg) = (cos(exponentArg), sin(exponentArg));
        const (cRhoPhiR, cRhoPhiI) = (realRhoPhi(kIndex), imagRhoPhi(kIndex));

        sOutR += cRhoPhiR * cosArg - cRhoPhiI * sinArg;
        sOutI += cRhoPhiI * cosArg + cRhoPhiR * sinArg;
        //kIndex += 1;
        //iIndex += 1;
      }
     
     
     (outR(xIndex), outI(xIndex)) = (sOutR, sOutI);
    }
  }
  */
    forall (sX, sY, sZ, sOutR, sOutI) in (x, y, z, outR, outI) {
      var (tmpOutR, tmpOutI) = (sOutR, sOutI);
      for kIndex in kIndex_range do {
      //for (KX, KY, KZ, cRhoPhiR, cRhoPhiI) in (kx, ky, kz, realRhoPhi, imagRhoPhi) do {
        //const exponentArg = PIx2:real(32) * (KX * sX + KY * sY + KZ * sZ);
        const exponentArg = PIx2:real(32) * (kx(kIndex) * sX + ky(kIndex) * sY + kz(kIndex) * sZ);
        const (cosArg, sinArg) = (cos(exponentArg), sin(exponentArg));
        //const (tcRhoPhiR, tcRhoPhiI) = (cRhoPhiR, cRhoPhiI);
        const (cRhoPhiR, cRhoPhiI) = (realRhoPhi(kIndex), imagRhoPhi(kIndex));
        tmpOutR += cRhoPhiR * cosArg - cRhoPhiI * sinArg;
        //tmpOutR += tcRhoPhiR * cosArg - tcRhoPhiI * sinArg;
        tmpOutI += cRhoPhiI * cosArg + cRhoPhiR * sinArg;
        //tmpOutI += tcRhoPhiI * cosArg + tcRhoPhiR * sinArg;
      }
      (sOutR, sOutI) = (tmpOutR, tmpOutI);
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
