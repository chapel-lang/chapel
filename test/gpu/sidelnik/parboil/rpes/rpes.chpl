use GPUDist, GPUSharedMemDist;

_extern def __expf(x : real(32)): real(32);
_extern def __fdividef(x : real(32), y : real(32)): real(32);
_extern def __fdividef(x : int(32), y : int(32)): int(32);
_extern def __mul24(x: int(32), y : int(32)) : int(32);

config var filename1 = "file.inp";
config var filename2 = "basis.inp";

param BLOCK_SIZE = 64;
param TABLESIZE = 2;
param W_MAX_SIZE = 10;
param GRID_SIZE = 65535;
param LOG_BLOCK_SIZE = 6;

record Shell {
  var Type : string;
  var numPrimitives : int;
  var Alpha : [0..19] real(32);
  var Coeff : [0..19] real(32);
  var inList : [0..19] int;
  var myAtom : int;
}

record Atom {
  var Type : string;
  var numShells : int;
  var AtomShell : [0..9] Shell;
  var X : real(32);
  var Y : real(32);
  var Z : real(32);
}

_extern record float4 {
  var x, y, z, w : real(32);
}

_extern record float3 {
  var x, y, z : real(32);
}

_extern record float2 {
  var x, y : real(32);
}

_extern record uint2 {
  var x, y : uint(32);
}
_extern record uint4 {
  var x, y, z, w : uint(32);
}

def main() {

  const GPUBlockDist = new dmap(new GPUDist(rank=1, tbSizeX=BLOCK_SIZE));
  const GPUSharedDist = new dmap(new GPUSharedMemDist(rank=1));

  var totNumAtoms = 0;
  var numBasisAtoms = 0;
  var totNumBlocks = 0;
  var MaxBlocks = 0;
  var BasisAtom = ReadBasisAtoms("basis.inp", numBasisAtoms);
  var totNumShells = TotalNumOfShells("file.inp", BasisAtom, numBasisAtoms, totNumAtoms);

  var totReductionElements : int = totNumShells * (totNumShells + 1) *
    (totNumShells + 2) * (totNumShells + 3) / 24;
  const reduction_space : domain(1, int) dmapped GPUBlockDist = 0..(totReductionElements-1);
  //const sub_reduce_space : domain(1, int) dmapped GPUBlockDist = 0..5;
  var ReductionSum : [reduction_space] real(32);
  var FinalReduce : [reduction_space] uint2;

  var ComputeAtom : [0..totNumAtoms-1] Atom;
  var ComputeShell : [0..totNumShells-1] Shell;
  PopulateShells("file.inp", BasisAtom, ComputeAtom, ComputeShell, numBasisAtoms);
  // all shells are ready now

  const TableSize_space : domain(1, int) dmapped GPUBlockDist = 0..(TABLESIZE - 1);
  var Wghts : [TableSize_space] real(32);

  // prepare host data
  var totBasisShells = 0;
  for i in 0..(numBasisAtoms-1) do {
    for j in 0..(BasisAtom(i).numShells-1) do {
      totBasisShells += BasisAtom(i).AtomShell(j).numPrimitives;
    }
  }
  
  const NumAtoms_space : domain(1, int) dmapped GPUBlockDist = 0..(totNumAtoms - 1);
  const BasisShells_space : domain(1, int) dmapped GPUBlockDist = 0..(totBasisShells - 1);
  var Coors : [NumAtoms_space] float4;
  var Sprms : [BasisShells_space] float2;

  PopulateHostData(totNumAtoms, totNumShells, numBasisAtoms, Coors, BasisAtom, Wghts, ComputeAtom, Sprms);

  //  distribute the work now
  var numIntegrals = NumOfIntegrals(totNumShells, ComputeShell, FinalReduce, totNumBlocks, MaxBlocks);
  writeln("Total # of integrals to compute: ", numIntegrals);
  writeln("Total # of blocks allocated: ", totNumBlocks);
  writeln("Final array size: ", totReductionElements);


  // prepare device data
  const NumBlocks_space : domain(1, int) dmapped GPUBlockDist = 0..(totNumBlocks - 1);
  var BlockSize_space : domain(1, int) dmapped GPUSharedDist = 0..(BLOCK_SIZE - 1);

  var Block_Work : [NumBlocks_space] uint4;
  var Output : [NumBlocks_space] real(32);

  DistributeBlockWork(totNumShells, ComputeShell, Block_Work);

  writeln("maxblocks = ", MaxBlocks, " size of int = ", numBytes(int), " size of int(32) = ", numBytes(int(32)), "size of int(64) = ", numBytes(int(64)));
  RunKernel(numIntegrals, totNumBlocks, Block_Work, Coors, Sprms, 
      BlockSize_space, Output, FinalReduce, 
      ReductionSum, MaxBlocks, totReductionElements, GPUBlockDist);

  //var outfile = new file("output_rpes", FileAccessMode.write);
  //var outfile2 = new file("output_rpes2", FileAccessMode.write);
  //outfile.open();
  //outfile2.open();

  for i in 0..19999 do {
    writeln(i, "\t", format("%e", ReductionSum(i)));
  }
//  for i in 0..(totNumBlocks - 1) do {
//    outfile2.writeln(i, "\t", format("%0.20f", Output(i)));
//  }
//  for i in 0..(totNumBlocks - 1) do {
    //outfile2.writeln(i, "\t", format("%u", Block_Work(i).x), " ", format("%u", Block_Work(i).y) , " ", format("%u", Block_Work(i).z) , " ", format("%u", Block_Work(i).w));
//    outfile2.writeln(i, "\t", Block_Work(i).x, " ", Block_Work(i).y, " ", Block_Work(i).z, " ", Block_Work(i).w);
//  }
//  outfile.close();
//  outfile2.close();
}


def RunKernel(numIntegrals, totNumBlocks, in Block_Work, in Coors, 
    in Sprms, BlockSize_space, out Output, 
    in FinalReduce, out ReductionSum, MaxBlocks, totReductionElements, 
    in GPUBlockDist) {
  var runs = ceil(1.0 : real(32) * totNumBlocks : real(32) / GRID_SIZE : real(32)) : int;
  writeln(runs, " computation cycles will be performed...");
  var RemainingBlocks = totNumBlocks;
  var StartBlock = 0;

  var Data : [BlockSize_space] real(32);
  var shared_size = BLOCK_SIZE * numBytes(real(32));

  var gpuSpace : domain(1, int) dmapped GPUBlockDist;
  for run in 0..#runs do {
    var numBlocks = min(GRID_SIZE, RemainingBlocks);
    // run kernel
    var Offset = StartBlock;

    writeln("run = ", run, " Offset = ", Offset, " numBlocks = ", numBlocks );

    gpuSpace = 0..(numBlocks*BLOCK_SIZE)-1;
    gpuSpace._value.sharedsize = shared_size;
    forall myIndex in gpuSpace do {

      const (blid, thid) = (getBlockID_x(), getThreadID_x());

      const myWorkIndex = (Offset << LOG_BLOCK_SIZE) + myIndex;
      const myWork = Block_Work(Offset + blid);

      var a1, a2, a3, a4, localthid : int;
      GetBases(myWork, myWorkIndex, localthid, a1, a2, a3, a4);

      var Result = 0.0 : real(32);
      //var Maxthid = a1 * a2 * a3 * a4;
      var Maxthid = __mul24(a1, __mul24(a2, __mul24(a3, a4)));
      if (localthid < Maxthid) then {
        var n1, n2, n3, n4 : int;
        var atom1, atom2, atom3, atom4 : int;
        var off1, off2, off3, off4 : int;
        GetAtoms(myWork, atom1, atom2, atom3, atom4);
        GetNs(localthid, a1, a2, a3, a4, n1, n2, n3, n4);
        GetOffsets(myWork, off1, off2, off3, off4);
        n1 += off1;
        n2 += off2;
        n3 += off3;
        n4 += off4;

        const (Atom1, Atom2, Atom3, Atom4) = (Coors(atom1), Coors(atom2), Coors(atom3), Coors(atom4));
        const (Param1, Param2, Param3, Param4) = (Sprms(n1), Sprms(n2), Sprms(n3), Sprms(n4));

        const (R12, R34) = (dist2(Atom1, Atom2), dist2(Atom3, Atom4));
        const (sum12, sum34) = (Param1.x + Param2.x, Param3.x + Param4.x);
        const (prod12, prod34) = (Param1.x * Param2.x, Param3.x * Param4.x);
        //const preexp = prod12 / sum12 * R12 + prod34 / sum34 * R34;
        const preexp = __fdividef(prod12, sum12) * R12 + __fdividef(prod34, sum34) * R34;
        //const preintegral = Param1.y * Param2.y * Param3.y * Param4.y * 
        //  __expf(-preexp) / (sum12 * sum34) * (1.0:real(32) / sqrt(sum12 + sum34));
        const preintegral = __fdividef(Param1.y * Param2.y * Param3.y * 
                                       Param4.y * __expf(- preexp), sum12 *
                                        sum34) * (1.0:real(32) / sqrt(sum12 + sum34));

        if(preintegral * preintegral > 1.0e-23 : real(32)) then {
          const (atompx, atompy, atompz) = ReducePair(Atom1, Atom2, Param1, Param2, sum12);
          const (atomqx, atomqy, atomqz) = ReducePair(Atom3, Atom4, Param3, Param4, sum34);
          const (dx, dy, dz) = (atompx - atomqx, atompy - atomqy, atompz - atomqz);
          //const (rpq2, rho)  = (dx * dx + dy * dy + dz * dz, sum12 * sum34 / (sum12 + sum34));
          const (rpq2, rho)  = (dx * dx + dy * dy + dz * dz, __fdividef(sum12 * sum34 , sum12 + sum34));
          Result = 34.98683666 : real(32) * preintegral * Root(rpq2 * rho);
        }
      }
      Data(thid) = Result;
      thread_barrier();

      //for s in sub_reduce_space do {
      for param s in 0..5 {
        const i = 1 << (s * -1 + LOG_BLOCK_SIZE - 1);
        if thid < i then
          Data(thid) += Data(thid+i);
        thread_barrier();
      }

      if(thid == 0) then
        Output(Offset + blid) = Data(0);
    }
    RemainingBlocks -= GRID_SIZE;
    StartBlock += numBlocks;
  }
  runs = ceil(1.0 : real(32) * totReductionElements : real(32) / GRID_SIZE : real(32)) : int;
  writeln("done.\n\n", runs, " reduction cycles will be performed...");
  var RemainReduction = totReductionElements;
  var Offset = 0;

  const GPUBlockDist_reduc = new dmap(new GPUDist(rank=1, tbSizeX=MaxBlocks));
  var gpuSpace_reduc : domain(1, int) dmapped GPUBlockDist_reduc;

  //runs = 0;
  for run in 0..#runs do {
    var numBlocks = min(GRID_SIZE, RemainReduction);
    gpuSpace_reduc = 0..(numBlocks * MaxBlocks)-1;
    gpuSpace_reduc._value.sharedsize = shared_size;

    //writeln("MaxBlocks = ", MaxBlocks, " numBlocks = ", numBlocks);

    // reduction kernel
    forall gpuSpace_reduc do {

      const (blid, thid) = (getBlockID_x(), getThreadID_x());
      const reduce_data = FinalReduce(Offset+blid);
      const (firstElement, offset) = (reduce_data.x, reduce_data.y);

      if thid < offset then
        Data(thid) = Output(firstElement : int + thid);
      else
        Data(thid) = 0.0 : real(32); 
      thread_barrier();

      for s in 0..5 do {
        const i = 1 << (s * -1 + LOG_BLOCK_SIZE - 1);
        if thid < i then
          Data(thid) += Data(thid+i);
        thread_barrier();
      }

      if thid == 0 then
        ReductionSum(Offset + blid) = Data(0);
    }

    RemainReduction -= GRID_SIZE;
    Offset += numBlocks;
  }
}

def Root(X : real(32)) {
  var WW1,F1,E,Y,inv: real(32);
  const rPIE4 = 1.273239545 : real(32);

  if (X < 3.0e-7:real(32)) then
    WW1 = 1.0 : real(32) - 0.333333333 : real(32) * X;
  else if (X < 1.0:real(32)) then {
    F1 = ((((((((-8.36313918003957E-08:real(32)*X+1.21222603512827E-06:real(32))*X-
                    1.15662609053481E-05:real(32) )*X+9.25197374512647E-05:real(32) )*X-
                6.40994113129432E-04:real(32) )*X+3.78787044215009E-03:real(32) )*X-
            1.85185172458485E-02:real(32) )*X+7.14285713298222E-02:real(32) )*X-
        1.99999999997023E-01:real(32) )*X+3.33333333333318E-01:real(32);
    WW1 = (X+X)*F1 + __expf(-X):real(32);
  }
  else if (X < 3.0:real(32)) then {
    Y = X-2.0:real(32);
    F1 = ((((((((((-1.61702782425558E-10:real(32)*Y+1.96215250865776E-09:real(32) )*Y-
                        2.14234468198419E-08:real(32) )*Y+2.17216556336318E-07:real(32) )*Y-
                    1.98850171329371E-06:real(32) )*Y+1.62429321438911E-05:real(32) )*Y-
                1.16740298039895E-04:real(32) )*Y+7.24888732052332E-04:real(32) )*Y-
            3.79490003707156E-03:real(32) )*Y+1.61723488664661E-02:real(32) )*Y-
        5.29428148329736E-02:real(32) )*Y+1.15702180856167E-01:real(32);
    WW1 = (X+X)*F1+__expf(-X):real(32);
  }
  else if (X < 5.0:real(32)) then {
    Y = X-4.0:real(32);
    F1 = ((((((((((-2.62453564772299E-11:real(32)*Y+3.24031041623823E-10:real(32) )*Y-
                        3.614965656163E-09:real(32))*Y+3.760256799971E-08:real(32))*Y-
                    3.553558319675E-07:real(32))*Y+3.022556449731E-06:real(32))*Y-
                2.290098979647E-05:real(32))*Y+1.526537461148E-04:real(32))*Y-
            8.81947375894379E-04:real(32))*Y+4.33207949514611E-03:real(32) )*Y-
        1.75257821619926E-02:real(32) )*Y+5.28406320615584E-02:real(32);
    WW1 = (X+X)*F1+__expf(-X):real(32);
  }
  else if (X < 10.0:real(32)) then {
    E = __expf(-X):real(32);
    inv = 1.0:real(32) / X;
    WW1 = (((((( 4.6897511375022E-01:real(32)*inv-6.9955602298985E-01:real(32))*inv +
                5.3689283271887E-01:real(32))*inv-3.2883030418398E-01:real(32))*inv +
            2.4645596956002E-01:real(32))*inv-4.9984072848436E-01:real(32))*inv -
        3.1501078774085E-06:real(32))*E + 1.0:real(32) / sqrt(rPIE4 * X) : real(32);
  } 
  else if (X < 15.0:real(32)) then {
    E = __expf(-X):real(32);
    inv = 1.0:real(32) / X;
    WW1 = (((-1.8784686463512E-01:real(32)*inv+2.2991849164985E-01:real(32))*inv -
          4.9893752514047E-01:real(32))*inv-2.1916512131607E-05:real(32))*E
      + 1.0:real(32) / sqrt(rPIE4 * X);
  }
  else if (X < 33.0:real(32)) then {
    E = __expf(-X):real(32);
    inv = 1.0:real(32) / X;
    WW1 = (( 1.9623264149430E-01:real(32)*inv-4.9695241464490E-01:real(32))*inv -
        6.0156581186481E-05:real(32))*E + 1.0:real(32) / sqrt(rPIE4 * X);
  }
  else
    WW1 = 1.0:real(32) / sqrt(rPIE4 * X);
  return WW1;
}

/*
def ReducePair(inout AtomP, in Atom1, in Atom2, in Param1, in Param2, sum_12) {
  AtomP.x = product1D(Param1.x, Atom1.x, Param2.x, Atom2.x, sum_12);
  AtomP.y = product1D(Param1.x, Atom1.y, Param2.x, Atom2.y, sum_12);
  AtomP.z = product1D(Param1.x, Atom1.z, Param2.x, Atom2.z, sum_12);
}
*/

//def ReducePair(in Atom1, in Atom2, in Param1, in Param2, sum_12) {
def ReducePair(Atom1, Atom2, Param1, Param2, sum_12) {
  var (param1x, param2x) = (Param1.x, Param2.x);
  var (atom1x, atom1y, atom1z) = (Atom1.x, Atom1.y, Atom1.z);
  var (atom2x, atom2y, atom2z) = (Atom2.x, Atom2.y, Atom2.z);
  /*
  var x = product1D(param1x, atom1x, param2x, atom2x, sum_12);
  var y = product1D(param1x, atom1y, param2x, atom2y, sum_12);
  var z = product1D(param1x, atom1z, param2x, atom2z, sum_12);
  */

 var x = __fdividef(param1x * atom1x + param2x * atom2x, sum_12);
 var y = __fdividef(param1x * atom1y + param2x * atom2y, sum_12);
 var z = __fdividef(param1x * atom1z + param2x * atom2z, sum_12);
 return (x,y,z);
}

pragma "inline" def product1D(alpha_a : real(32), coor_a : real(32), alpha_b : real(32), coor_b : real(32), sum_ab : real(32)) {
//def product1D(alpha_a, coor_a, alpha_b, coor_b, sum_ab) {
  return __fdividef(alpha_a * coor_a + alpha_b * coor_b, sum_ab);
  //return ((alpha_a * coor_a + alpha_b * coor_b) / sum_ab);
}

def dist2(in A1, in A2) {
  const (dx, dy, dz) = (A1.x - A2.x, A1.y - A2.y, A1.z - A2.z);
  return dx * dx + dy * dy + dz * dz;
}

def GetNs(localthid, a1, a2, a3, a4, out n1, out n2, out n3, out n4) {
  var prod = __mul24(a2, __mul24(a3, a4));
  var res = localthid;
  n1 = __fdividef(res, prod);
  res -= __mul24(n1, prod);
      
  prod = __mul24(a3, a4);
  n2 = __fdividef(res, prod);
  res -= __mul24(n2, prod);

  prod = a4;
  n3 = __fdividef(res, prod);
  res -= __mul24(n3, prod);

  n4 = res;
}

def GetBases(_Work, thid, out localthid, out a1, out a2, out a3, out a4) {
  const Work = _Work.x;
  const Shift = _Work.w;
  a1 = ((Work & 61440 : uint(32)) >> 12) : int;
  a2 = ((Work & 3840  : uint(32)) >> 8)  : int;
  a3 = ((Work & 240   : uint(32)) >> 4)  : int;
  a4 = ((Work & 15    : uint(32)))       : int;
  localthid = (thid - (Shift << LOG_BLOCK_SIZE)) : int;
}

def GetAtoms(_Work, out atom1, out atom2, out atom3, out atom4) {
  const Work = _Work.y;
  atom1 = ((Work & (4278190080 : uint(32))) >> 24) : int;
  atom2 = ((Work & (16711680   : uint(32))) >> 16) : int;
  atom3 = ((Work & (65280      : uint(32))) >> 8)  : int;
  atom4 = (Work  & (255        : uint(32))) : int;
}

def GetOffsets(_Work, out off1, out off2, out off3, out off4) {
  const Work = _Work.z;
  off1 = ((Work & (4278190080 : uint(32))) >> 24) : int;
  off2 = ((Work & (16711680   : uint(32))) >> 16) : int;
  off3 = ((Work & (65280      : uint(32))) >> 8)  : int;
  off4 = (Work  & (255        : uint(32)))        : int;
}

def DistributeBlockWork(totNumShells, in ComputeShell, out Block_Work) {
  var numElements = 0;
  var StartBlock = 0;
  for shell1 in 0..(totNumShells-1) do {
    for shell2 in shell1..(totNumShells-1) do {
      for shell3 in shell2..(totNumShells-1) do {
        for shell4 in shell3..(totNumShells-1) do {
          var integrals = ComputeShell(shell1).numPrimitives * 
            ComputeShell(shell2).numPrimitives *
            ComputeShell(shell3).numPrimitives *
            ComputeShell(shell4).numPrimitives;

          var blocks = ceil(1.0 : real(32) * integrals : real(32) / BLOCK_SIZE : real(32)) : int;
          StartBlock = numElements;

          for block in 0..(blocks-1) do {
            var a4 = ComputeShell(shell4).numPrimitives;
            var a3 = ComputeShell(shell3).numPrimitives;
            var a2 = ComputeShell(shell2).numPrimitives;
            var a1 = ComputeShell(shell1).numPrimitives;

            var offset4 = ComputeShell(shell4).inList(0);
            var offset3 = ComputeShell(shell3).inList(0);
            var offset2 = ComputeShell(shell2).inList(0);
            var offset1 = ComputeShell(shell1).inList(0);

            Block_Work(numElements).y =
              ((ComputeShell(shell1).myAtom << 24) |
              (ComputeShell(shell2).myAtom << 16) |
              (ComputeShell(shell3).myAtom << 8 ) |
              (ComputeShell(shell4).myAtom      )) : uint(32);

            Block_Work(numElements).z =
              ((offset1 << 24) |
              (offset2 << 16) |
              (offset3 << 8 ) |
              (offset4)) : uint(32);

            Block_Work(numElements).x =
              ((a1         << 12) |
              (a2         <<  8) |
              (a3         <<  4) |
              (a4              )) : uint(32);

            Block_Work(numElements).w = StartBlock : uint(32);

            numElements +=1;
          }
        }
      }
    }
  }
}


def NumOfIntegrals(totNumShells, in ComputeShell, inout FinalReduce, inout totNumBlocks, inout MaxBlocks) {
  var numIntegrals = 0;
  var firstRedElement = 0;
  var redElement = 0;
  for shell1 in 0..(totNumShells-1) do {
    for shell2 in shell1..(totNumShells-1) do {
      for shell3 in shell2..(totNumShells-1) do {
        for shell4 in shell3..(totNumShells-1) do {
          var integrals = ComputeShell(shell1).numPrimitives * 
            ComputeShell(shell2).numPrimitives *
            ComputeShell(shell3).numPrimitives *
            ComputeShell(shell4).numPrimitives;
          numIntegrals += integrals;
          var blocks = ceil(1.0 : real(32) * integrals : real(32) / BLOCK_SIZE : real(32)) : int;
          totNumBlocks += blocks;
          if (blocks > MaxBlocks) then {
            MaxBlocks = blocks;
          }
          FinalReduce(redElement).x = firstRedElement : uint;
          FinalReduce(redElement).y = blocks : uint;
          firstRedElement += blocks;
          redElement += 1;
        }
      }
    }
  }
  return numIntegrals;
}

def PopulateHostData(totNumAtoms, totNumShells, numBasisAtoms, out Coors, in BasisAtom, out Wghts, in ComputeAtom, out Sprms) {
 // PopulateWeights(Wghts);
  for atom in 0..(totNumAtoms-1) do {
    Coors(atom).x = ComputeAtom(atom).X;
    Coors(atom).y = ComputeAtom(atom).Y;
    Coors(atom).z = ComputeAtom(atom).Z;
  }
  var currentPos = 0;
  for batom in 0..(numBasisAtoms-1) do {
    for shell in 0..(BasisAtom(batom).numShells-1) do {
      for prim in 0..(BasisAtom(batom).AtomShell(shell).numPrimitives-1) do {
        Sprms(currentPos).x = BasisAtom(batom).AtomShell(shell).Alpha(prim);
        Sprms(currentPos).y = BasisAtom(batom).AtomShell(shell).Coeff(prim);
        currentPos += 1;
      }
    }
  }
}

def PopulateWeights(out Wghts) {
  for i in 0..(TABLESIZE-1) do {
    var X : real(32) = (1.0 : real(32) * i * W_MAX_SIZE / (TABLESIZE - 1)) : real(32);
    Wghts(i) = root1(X:real(64)) : real(32);
  }
}

def root1(X : real(64)) {
  var F1, E, Y, inv : real(64);
  var WW1 = 0.0 : real(64);
  var PIE4 = 7.85398163397448E-01 : real(64);
  if (X < 3.0e-7) then {
    WW1 = 1.0 - 0.333333333 * X;
  }
  else if (X < 1.0) then {
      F1 = ((((((((-8.36313918003957E-08*X+1.21222603512827E-06 )*X-
      1.15662609053481E-05 )*X+9.25197374512647E-05 )*X-
    6.40994113129432E-04 )*X+3.78787044215009E-03 )*X-
        1.85185172458485E-02 )*X+7.14285713298222E-02 )*X-
      1.99999999997023E-01 )*X+3.33333333333318E-01;
      WW1 = (X+X)*F1 + exp(-X):real(32);
  }
  else if (X < 3.0) {
      Y = X-2.0;
      F1 = ((((((((((-1.61702782425558E-10*Y+1.96215250865776E-09 )*Y-
        2.14234468198419E-08 )*Y+2.17216556336318E-07 )*Y-
      1.98850171329371E-06 )*Y+1.62429321438911E-05 )*Y-
    1.16740298039895E-04 )*Y+7.24888732052332E-04 )*Y-
        3.79490003707156E-03 )*Y+1.61723488664661E-02 )*Y-
      5.29428148329736E-02 )*Y+1.15702180856167E-01;
      WW1 = (X+X)*F1+exp(-X):real(32);
  }
  else if (X < 5.0) {
      Y = X-4.0;
      F1 = ((((((((((-2.62453564772299E-11*Y+3.24031041623823E-10 )*Y-
        3.614965656163E-09)*Y+3.760256799971E-08)*Y-
      3.553558319675E-07)*Y+3.022556449731E-06)*Y-
    2.290098979647E-05)*Y+1.526537461148E-04)*Y-
        8.81947375894379E-04)*Y+4.33207949514611E-03 )*Y-
      1.75257821619926E-02 )*Y+5.28406320615584E-02;
      WW1 = (X+X)*F1+exp(-X):real(32);
  }
  else if (X < 10.0) {
      E = exp(-X):real(32);
      inv = 1.0:real(32) / X;
      WW1 = (((((( 4.6897511375022E-01*inv-6.9955602298985E-01)*inv +
     5.3689283271887E-01)*inv-3.2883030418398E-01)*inv +
         2.4645596956002E-01)*inv-4.9984072848436E-01)*inv -
       3.1501078774085E-06)*E + sqrt(PIE4*inv);
  }
  else if (X < 15.0) {
      E = exp(-X):real(32);
      inv = 1.0:real(32) / X;
      WW1 = (((-1.8784686463512E-01*inv+2.2991849164985E-01)*inv -
        4.9893752514047E-01)*inv-2.1916512131607E-05)*E + sqrt(PIE4*inv);
  }
  else if (X < 33.0) {
      E = exp(-X);
      inv = 1 / X;
      WW1 = (( 1.9623264149430E-01*inv-4.9695241464490E-01)*inv -
       6.0156581186481E-05)*E + sqrt(PIE4*inv);
  }
  else {
      inv = 1 / X;
      WW1 = sqrt(PIE4*inv);
  }
  
  return WW1;
}

def PopulateShells(filename, in BasisAtom, out ComputeAtom, out ComputeShell, numBasisAtoms) {
  var infile = new file(filename, FileAccessMode.read);
  infile.open();
  var (temp, numAtoms) = infile.readln(string, int);
  
  var currentShell = 0;
  for atom in 0..(numAtoms-1) do {
    var (atomType, X, Y, Z) = infile.readln(string, real(32), real(32), real(32));
    ComputeAtom(atom).Type = atomType;
    ComputeAtom(atom).X = X : real(32);
    ComputeAtom(atom).Y = Y : real(32);
    ComputeAtom(atom).Z = Z : real(32);

    var currentInList = 0;
    for batom in 0..(numBasisAtoms-1) do {
      if (BasisAtom(batom).Type == ComputeAtom(atom).Type) then {
        for shell in 0..(BasisAtom(batom).numShells-1) {
          ComputeShell(currentShell) = BasisAtom(batom).AtomShell(shell);
          ComputeShell(currentShell).myAtom = atom;

          for prim in 0..(BasisAtom(batom).AtomShell(shell).numPrimitives-1) do {
            ComputeShell(currentShell).inList(prim) = currentInList;
            currentInList += 1;
          }
          currentShell += 1;
        }
        break;
      }
      else {
        for shell in 0..(BasisAtom(batom).numShells-1) do
          currentInList += BasisAtom(batom).AtomShell(shell).numPrimitives;
      }
    }
  }
  infile.close();
}

def TotalNumOfShells(filename, in BasisAtom, in numBasisAtoms, out totNumAtoms) {
  var infile = new file(filename, FileAccessMode.read);
  infile.open();
  var numShells = 0;
  var (temp, totatoms) = infile.readln(string, int);
  totNumAtoms = totatoms;

  for atom in 0..(totNumAtoms-1) do {
    var (atom_type, temp, temp1, temp2) = infile.readln(string, string, string, string);
    var notfound = 1;
    for batom in 0..(numBasisAtoms-1) do {
      
      if (BasisAtom(batom).Type == atom_type) then {
        numShells += BasisAtom(batom).numShells;
        notfound = 0;
        break;
      }
    }
    if (notfound == 1) then {
      halt("Unable to find atom '", atom_type, "' in the basis set");
    }
  }
  infile.close();
  return numShells;
}

def ReadBasisAtoms(filename, out numBasisAtoms) {
  var infile = new file(filename, FileAccessMode.read);
  infile.open();
  var (temp, temp1, numAtoms) = infile.readln(string, string, int);
  var (temp2, temp3, numShells) = infile.readln(string, string, int);
  writeln("\n>>>>>>> STARTED BASIS SET OUTPUT <<<<<<<");
  writeln("\n# OF KNOWN ATOMS: ", numAtoms);
  writeln("# OF KNOWN SHELLS: ", numShells);

  numBasisAtoms = numAtoms;
  var BasisAtom : [0..#numAtoms] Atom;

  for basisatom in BasisAtom do {
    var (temp, atom_type) = infile.readln(string, string);
    var (temp1, numShells) = infile.readln(string, int);
    basisatom.numShells = numShells;
    basisatom.Type = atom_type;
    for shell in 0..(numShells-1) do {
      var (temp, temp1, temp2, numPrimitives) = infile.readln(string, int, string, int);
      basisatom.AtomShell(shell).numPrimitives = numPrimitives;
      var temp_str = atom_type + (shell + 1) : string;
      basisatom.AtomShell(shell).Type = temp_str;
      writeln("\tShell ", basisatom.AtomShell(shell).Type, ": ", basisatom.AtomShell(shell).numPrimitives," primitives");
      for prim in 0..(numPrimitives - 1) do {
        var (temp, temp1, temp2, alpha, coeff) = infile.readln(string, string, string, real(32), real(32));
        basisatom.AtomShell(shell).Alpha(prim) = alpha : real(32);
        basisatom.AtomShell(shell).Coeff(prim) = coeff : real(32);
        writeln("\t\tprimitive ", prim+1,": ",basisatom.AtomShell(shell).Alpha(prim),"    ", basisatom.AtomShell(shell).Coeff(prim));
      }
      writeln();
    }
  }

  writeln(">>>>>>>> DONE BASIS SET OUTPUT <<<<<<<<\n\n");
  infile.close();
  return BasisAtom;
}
