// This is an implementation of the MultiGrid (MG) NAS Parallel Benchmark.
//
// Optimizations (in order)
//   -- start with the most concise/abstract solution
//   -- factor out the stencil convolution operation, so that we might speed that up.
//   -- use the Stencil distribution
//   -- NOTE : We will make a number of optimizations assuming that the third dimension is 
//             fully local. TODO : This should be enforced in the dmapped statement itself.
use Time;
use StencilDist;

// Definitions of NPB parameters etc
enum NPB {S,A,B,C}; // TODO : Make complete.
const ProblemSizes : [NPB.S..NPB.C] int = [32, 256, 256, 512],
      ProblemIters : [NPB.S..NPB.C] int = [4, 4, 20, 20],
      ExpectedResids : [NPB.S..NPB.C] real = [0.5307707005735e-04, 
                                              0.2433365309069e-05,
                                              0.1800564401355e-05,
                                              0.5706732285705e-06],
      fracGoal = 1.0e-8;

config const NPBClass : NPB = NPB.S;
config const debug=false;
config const verifyOnly=false;

param Ndim=3;


const N=ProblemSizes[NPBClass],
      numlevels=log2(N),
      MGStencil={-1..1,
                 -1..1,
                 -1..1},
      HaloSize = (1,1,1),
      LevelDom = {1..numlevels};

type coeff = [0..3] real; 

// Initialize levels
class MGLevel {
  var n1 : int;
  var dom = {0.. #n1, 0.. #n1, 0.. #n1} dmapped Stencil({0.. #n1, 0.. #n1, 0.. #n1},
                            fluff=HaloSize,
                            periodic=true);
  var Z, R : [dom] real;
}

var fluffTime : Timer;

proc main() {
  // Allocate the levels 
  var Levels : [LevelDom] MGLevel;
  for ilevel in LevelDom do Levels[ilevel] = new MGLevel(2**ilevel);

  var U,V,R : [Levels[numlevels].dom] real;

  writeln("NPB Multigrid Benchmark");
  writeln("Running class : ", NPBClass);

  fillInit(U, V);
  
  var timeit : Timer;
  timeit.clear();
  timeit.start();
  fluffTime.clear();

  for ii in 1..ProblemIters[NPBClass] {
    mgstep(U,V, Levels);
  }

  // Final residual calculation
  resid(R,V,U);
  debugPrint("Time before final norm calculation :",timeit.elapsed());
  var nR = norm(R),
      verify=ExpectedResids[NPBClass],
      err = (nR-verify)/verify;

  timeit.stop();

  if !verifyOnly {
    writef("The L2 norm of the residual is : %20.15er\n",nR);
    writef("The expected L2 norm of the residual is : %20.15er\n",verify);
    writef("The fractional error is : %20.15er\n",err);
  }

  if (err < fracGoal) {
    writeln("VERIFICATION SUCCESSFUL");
  } else {
    writeln("VERIFICATION FAILED");
  }
  if !verifyOnly {
    writeln("Elapsed time (seconds):", timeit.elapsed());
    writeln("Fluff time :", fluffTime.elapsed());
  }

  for il in Levels do delete il;
  
}




/* Multigrid step 
*/
proc mgstep(U, V, Levels) {
  var t1 : Timer;
  t1.clear();

  t1.start();
  resid(Levels[numlevels].R,V,U);
  debugPrint("Elapsed time after first residual :",t1.elapsed());

  for il in 2..numlevels by -1 do restrict(Levels[il-1].R, Levels[il].R);
  debugPrint("Elapsed time after restrictions :",t1.elapsed());
  
  Levels[1].Z = 0.0;
  smooth(Levels[1].Z, Levels[1].R);

  for il in 2..numlevels {
    prolong(Levels[il].Z, Levels[il-1].Z);
    debugPrint("Elapsed time prolonging :", t1.elapsed());
    resid(Levels[il].R, Levels[il].Z);
    debugPrint("Elapsed time after residual :", t1.elapsed());
    smooth(Levels[il].Z, Levels[il].R);
    debugPrint("Elapsed time in upward V cycle :",t1.elapsed());
  }

  // NOTE : This increment runs quite slowly in single locale mode.
  // Since this is an easy operation, do it directly...
  //U += Levels[numlevels].Z;
  increment(Levels[numlevels].Z, U);
  debugPrint("Completed V cycle :",t1.elapsed());
}

proc norm(R) : real {
  return sqrt((+ reduce R**2)/(N:real**3));
}

/* 
   Restriction operation 

   fine is input, coarse is output
 */
proc restrict(coarse:[?coarseDom] real, fine : [?FineDom]real) {
  const w : coeff = [1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/16.0];
  fine.updateFluff();

  forall (i,j,k) in coarseDom {
    const i2 = 2*i,
          j2 = 2*j,
          k2 = 2*k;
    var tmp = 0.5 * fine.localAccess[i2,j2,k2];
    tmp += 0.25 * (fine.localAccess[i2+1,j2,k2] + fine.localAccess[i2-1,j2,k2]
                 + fine.localAccess[i2,j2+1,k2] + fine.localAccess[i2,j2-1,k2]
                 + fine.localAccess[i2,j2,k2+1] + fine.localAccess[i2,j2,k2-1]);
    tmp += 0.1250 *(fine.localAccess[i2-1, j2-1, k2+0] +
                    fine.localAccess[i2-1, j2+0, k2-1] +
                    fine.localAccess[i2-1, j2+0, k2+1] +
                    fine.localAccess[i2-1, j2+1, k2+0] +
                    fine.localAccess[i2+0, j2-1, k2-1] +
                    fine.localAccess[i2+0, j2-1, k2+1] +
                    fine.localAccess[i2+0, j2+1, k2-1] +
                    fine.localAccess[i2+0, j2+1, k2+1] +
                    fine.localAccess[i2+1, j2-1, k2+0] +
                    fine.localAccess[i2+1, j2+0, k2+1] +
                    fine.localAccess[i2+1, j2+1, k2+0] +
                    fine.localAccess[i2+1, j2+0, k2-1]);
    tmp += 0.0625 *(fine.localAccess[i2-1, j2-1, k2-1] +
                    fine.localAccess[i2-1, j2-1, k2+1] +
                    fine.localAccess[i2-1, j2+1, k2-1] +
                    fine.localAccess[i2-1, j2+1, k2+1] +
                    fine.localAccess[i2+1, j2-1, k2-1] +
                    fine.localAccess[i2+1, j2-1, k2+1] +
                    fine.localAccess[i2+1, j2+1, k2-1] +
                    fine.localAccess[i2+1, j2+1, k2+1]);
    coarse.localAccess[i,j,k] = tmp;
  }
}

/* 
 Smoothing operator 

 z = z + S r
*/
proc smooth(Z :[?Dom] real, R : []real) {
  // The smoothing operator is defined differently for different classes. This is 
  // the S(a) definition
  const w : coeff = smoothingCoeff(NPBClass);

  // Do the actual stencil convolution
  stencilConvolve(Z, R, w, true);
}

/* 
 Residual operator 

 r = r - A z
*/
proc resid(R:[?Dom]real, V:[]real, Z:[]real) {
  // Negative of the coefficients, since always subtract
  const w : coeff = [8.0/3.0, 0.0, -1.0/6.0, -1.0/12.0];
  // TODO : Why does this not optimize properly???
  // R = V;
  [ijk in Dom] R.localAccess[ijk] = V.localAccess[ijk];
  stencilConvolve(R,Z,w,true);
}
// Overload when V = R
proc resid(R:[?Dom]real, Z:[]real) {
  // Negative of the coefficients, since always subtract
  const w : coeff = [8.0/3.0, 0.0, -1.0/6.0, -1.0/12.0];
  stencilConvolve(R,Z,w,true);
}

/* 
 Prolongation operator 

 We do this in a simple manner, which requires creating a temporary array. It can also 
 be easily optimized at the cost of more code.
*/
proc prolong(fine:[?fineDom]real, coarse:[?coarseDom]real) {
  const dom = {0..1,0..1,0..1};
  coarse.updateFluff();
  // TODO : This is a horrific piece of code.
  forall (i,j,k) in coarseDom {
    const i2 = 2*i,
          j2 = 2*j,
          k2 = 2*k;
    //var y : [dom] real;
    const y000 = coarse.localAccess[i,j,k],
          y001 = coarse.localAccess[i,j,k+1],
          y010 = coarse.localAccess[i,j+1,k],
          y100 = coarse.localAccess[i+1,j,k],
          y110 = coarse.localAccess[i+1,j+1,k],
          y101 = coarse.localAccess[i+1,j,k+1],
          y011 = coarse.localAccess[i,j+1,k+1],
          y111 = coarse.localAccess[i+1,j+1,k+1];
    //for (i1,j1,k1) in dom do y[i1,j1,k1] = coarse.localAccess[i+i1,j+j1,k+k1];
    fine.localAccess[i2,j2,k2] = y000;
    const yx = y000+y100,
          yy = y000+y010,
          yz = y000+y001,
          yyx = y010+y110,
          yzy = y001+y011,
          yxz = y100+y101;
    fine.localAccess[i2+1,j2,k2] = 0.5*yx;
    fine.localAccess[i2,j2+1,k2] = 0.5*yy;
    fine.localAccess[i2,j2,k2+1] = 0.5*yz;
    fine.localAccess[i2+1,j2+1,k2] = 0.25*(yx+yyx);
    fine.localAccess[i2,j2+1,k2+1] = 0.25*(yy+yzy);
    fine.localAccess[i2+1,j2,k2+1] = 0.25*(yz+yxz);
    fine.localAccess[i2+1,j2+1,k2+1] = 0.125*(yz+yyx+yxz+y011+y111);
  }

}


inline proc stencilConvolve(dest : [?Dom] real, const ref src : []real, const w : coeff, 
    param inc : bool = false) {
  fluffTime.start();
  src.updateFluff();
  fluffTime.stop();
  // Do the actual stencil convolution
  coforall loc in dest.targetLocales() {
    on loc {
      const w0 = w[0],
            w1 = w[1],
            w2 = w[2],
            w3 = w[3];
      local {
        var locdom = dest.localSubdomain();
        var locdom2 = {locdom.dim(1),locdom.dim(2)},
            locdom3 = locdom.dim(3);
        const klo = locdom3.low,
              khi = locdom3.high;
        forall (i1,j1) in locdom2  {
          // Zero
          dest.localAccess[i1,j1,klo-1] = 0.0; 
          dest.localAccess[i1,j1,khi+1] = 0.0;
          if !inc {
            [k1 in locdom3] dest.localAccess[i1,j1,k1] = 0.0;
          }
          for k1 in vectorizeOnly(klo..khi) {
            var val = src.localAccess[i1,j1,k1];
            var val1 = src.localAccess[i1+1,j1,k1]+src.localAccess[i1-1,j1,k1]+
              src.localAccess[i1,j1+1,k1]+src.localAccess[i1,j1-1,k1];
            var val2 = src.localAccess[i1+1,j1+1,k1]+src.localAccess[i1-1,j1+1,k1]+
              src.localAccess[i1+1,j1-1,k1]+src.localAccess[i1-1,j1-1,k1];
            dest.localAccess[i1,j1,k1] += w0*val + w1*val1 + w2*val2;
            var tmp = w1*val + w2*val1 + w3*val2;
            dest.localAccess[i1,j1,k1-1] += tmp;
            dest.localAccess[i1,j1,k1+1] += tmp;
          }
          // Handle periodic boundary conditions
          dest.localAccess[i1,j1,klo] += dest.localAccess[i1,j1,khi+1];
          dest.localAccess[i1,j1,khi] += dest.localAccess[i1,j1,klo-1]; 
        }
      }
    }
  }
}


// This is hardcoded, for simplicity.
proc fillInit(U, V : [?Dom]) {
  // Different cases for different classes
  // This could be generated on the fly, but it's simpler to just copy over the 
  // correct values, since there are just 20 points.
  var negative, positive : [1..10] Ndim*int;

  // These are straight from the benchmark, and so are 1-indexed
  select N {
    when 32 {
      negative  = [
        (  9, 16, 28),( 10, 27, 24),( 28, 17, 1),( 22, 21, 13),(  7, 19, 10),
        ( 14,  4,  3),(  6, 30, 17),(  7, 16,  2),( 15, 10, 19),(  2, 13,  4)];
      positive = [
        ( 19, 28, 19),( 30,  2, 30),( 32,  6, 27),( 14, 17, 14),( 23, 1,  8),
        (  3, 18, 23),(  6, 24,  5),(  4,  2,  5),( 21, 31, 1),(  9,  3, 22)];
    }
    when 256 {
      negative = [
        (211,154, 98),(102,138,112),(101,156, 59),( 17,205, 32),( 92, 63,205),
        (199,  7,203),(250,170,157),( 82,184,255),(154,162, 36),(223, 42,240)];
      positive = [
        ( 57,120,167),(  5,118,175),(176,246,164),( 45,194,234),(212,  7,248),
        (115,123,207),(202, 83,209),(203, 18,198),(243,172, 14),( 54,209, 40)];
    }
    when 512 {
      negative = [
        ( 74,  2,107),(402,504,449),(154,338, 10),( 18, 21,457),(352,194,418),
        (383, 74,283),(344,139,168),(223,278, 61),( 96,401,238),(399,312,200)];
      positive = [
        (151,401,331),(203, 10, 51),(146, 93,312),(163,280, 75),(509, 43,127),
        (243, 87,  5),(149,117,199),(451,270,443),( 11,493,118),(310,361, 11)];
    }
    otherwise {
      halt("Unknown dimension of array when setting initial values....");
    }
  }

  V = 0.0;
  U = 0.0;
  [ijk in negative] V[ijk-1] = -1.0;
  [ijk in positive] V[ijk-1] = 1.0;
}

inline proc increment(src : [?Dom]real, dest : []real) {
  forall ijk in Dom do dest.localAccess[ijk] += src.localAccess[ijk];
}
  

inline proc debugPrint(x...) {
  if debug then writeln((...x));
}

inline proc smoothingCoeff(c : NPB) {
  if c <= NPB.A {
    return [-3.0/8.0, 1.0/32.0, -1.0/64.0, 0.0];
  } else {
    return [-3.0/17.0, 1.0/33.0, -1.0/61.0, 0.0];
  }
}
