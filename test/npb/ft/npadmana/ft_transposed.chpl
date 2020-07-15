/*
  A Chapel implementation of the NPB FT Benchmark, using the
  DistributedFFT routines in this module.

  DistributedFFT implements a slab-decomposed FFT, using FFTW for all the
  local steps, but it does the transpose communication using Chapel.

  The goal of this benchmark is to a) verify that the FFT is indeed correctly
  implemented, and b) to compare its performance to FT benchmark.


*/
use DistributedFFT;
use Time;

// Define the classes
enum NPB {S,W,A,B,C,D,E,F};
config const NPBClass : NPB = NPB.S;

// Constants
const alpha = 1.0e-6;
config const Threshold = 1.0e-12;


// Define the domain
// Note that the definitions in the NPB documentation are
// Fortran-ordered, so we transpose the x and z variables
const (Nz, Ny, Nx) = ProblemSizes(NPBClass);
const Dom = newSlabDom((Nx,Ny,Nz));
const DomT = newSlabDom((Ny,Nx,Nz));

writeln("NPB FT Benchmark, Class=",NPBClass);
writef("Running on numLocales = %i\n",numLocales);
writef("Problem size = %i x %i x %i \n",Nx,Ny,Nz);


// Define arrays
var W : [Dom] complex;
var V, Wt: [DomT] complex;
var Twiddle : [DomT] real;

var timeit : Timer;

// Touch the arrays once
timeit.clear(); timeit.start();
initialize_U();
initialize_twiddle();
evolve();
timeit.stop();
writef("Time to touch arrays once : %10.4dr \n",timeit.elapsed());

// Now do the actual evolution
const refChecksums = ReferenceChecksums(NPBClass);
// Start timers
timeit.clear(); timeit.start();
initialize_U();
initialize_twiddle();

for ii in refChecksums.domain {
  evolve();
  const c1 = checksum();
  const verified = verify(c1, refChecksums[ii]);
  if !verified then halt("Checksum verification failed, halting.....");
  writef("Checksum(%0i) =  %14.12ez\n",ii,c1);
}
timeit.stop();

writeln("Successful completion of NPB FT Benchmark Class ",NPBClass);
const total_time = timeit.elapsed();
const ntotal_f = (Nx*Ny*Nz):real;
const niter = refChecksums.size;
const mflops = 1.0e-6*ntotal_f *
  (14.8157+7.19641*log(ntotal_f)
   +  (5.23518+7.21113*log(ntotal_f))*niter)
  /total_time;
writef("Elapsed time : %10.4dr\n",timeit.elapsed());
writef("MFLOPS : %10.4dr\n",mflops);

////////////////////////////////////////////////////
// Benchmark ends here
///////////////////////////////////////////////////


proc evolve() {
  forall ijk in DomT {
    const elt = V[ijk]*Twiddle[ijk];
    V[ijk] = elt;
    Wt[ijk] = elt;
  }
  doFFT_Transposed(FFTtype.DFT, Wt, W, FFTW_BACKWARD); // This is unnormalized

}

/* A few comments are in order here.

   The NPB specification defines j as running from 0 to 1023,
   but examining the code shows that it runs from 1 to 1024.

   Interestingly, for the S->C classes, these seem to yield the same value;
   however, going into the D class breaks things. We conform to the code
   version, since the checksums are based on that.

   Also, we follow FFTW and keep the backward transform unnormalized.
   The reference version also does that, and just normalized the
   checksum.
*/
proc checksum() : complex {
  const FTnorm = 1.0/(Nx*Ny*Nz):real;
  var ck : complex = 0.0 + 0.0i;
  forall j in 1..1024 with (+ reduce ck) {
    const q = (5*j)%Nx;
    const r = (3*j)%Ny;
    const s = j%Nz;
    ck += W[q,r,s];
  }
  return ck*FTnorm;
}


proc initialize_twiddle() {
  const Ns = (Ny, Nx, Nz);
  const fac = 4.0*pi**2 * alpha;
  forall xyz in DomT {
    var e : int = 0;
    for (idx, N) in zip(xyz, Ns) {
      const x1 = if idx >= N/2 then idx-N else idx;
      e += x1**2;
    }
    Twiddle[xyz] = exp(-fac*e);
  }
}

// This initializes the U array. Since that is never used,
// we store it in V.
proc initialize_U() {
  use Random;
  fillRandom(W, 314159265, RNG.NPB);
  doFFT_Transposed(FFTtype.DFT, W, V, FFTW_FORWARD);
}

proc verify(x : complex, y : complex) : bool {
  const diffre = abs((x.re - y.re)/y.re) < Threshold;
  const diffim = abs((x.im - y.im)/y.im) < Threshold;
  return diffre && diffim;
}

///-----------------------------------------------------///
// Problem definitions and checksums go here.

proc ProblemSizes(cc : NPB) : (3*int) {
  select cc {
      when NPB.S do return (64,64,64);
      when NPB.W do return (128, 128, 32);
      when NPB.A do return (256, 256, 128);
      when NPB.B do return (512, 256, 256);
      when NPB.C do return (512, 512, 512);
      when NPB.D do return (2048, 1024, 1024);
      when NPB.E do return (4096, 2048, 2048);
      when NPB.F do return (8192, 4096, 4096);
      otherwise do halt("Unknown class");
  }
}

proc ReferenceChecksums(cc : NPB) {
  select cc {
      when NPB.S do return
                      [(5.546087004964e+02, 4.845363331978e+02): complex,
                       (5.546385409189e+02, 4.865304269511e+02): complex,
                       (5.546148406171e+02, 4.883910722336e+02): complex,
                       (5.545423607415e+02, 4.901273169046e+02): complex,
                       (5.544255039624e+02, 4.917475857993e+02): complex,
                       (5.542683411902e+02, 4.932597244941e+02): complex];
      when NPB.W do return 
                      [(5.673612178944e+02, 5.293246849175e+02): complex,
                       (5.631436885271e+02, 5.282149986629e+02): complex,
                       (5.594024089970e+02, 5.270996558037e+02): complex,
                       (5.560698047020e+02, 5.260027904925e+02): complex,
                       (5.530898991250e+02, 5.249400845633e+02): complex,
                       (5.504159734538e+02, 5.239212247086e+02): complex];
      when NPB.A do return 
                      [(5.046735008193e+02, 5.114047905510e+02): complex,
                       (5.059412319734e+02, 5.098809666433e+02): complex,
                       (5.069376896287e+02, 5.098144042213e+02): complex,
                       (5.077892868474e+02, 5.101336130759e+02): complex,
                       (5.085233095391e+02, 5.104914655194e+02): complex,
                       (5.091487099959e+02, 5.107917842803e+02): complex];
      when NPB.B do return 
                      [(5.177643571579e+02, 5.077803458597e+02): complex,
                       (5.154521291263e+02, 5.088249431599e+02): complex,
                       (5.146409228649e+02, 5.096208912659e+02): complex,
                       (5.142378756213e+02, 5.101023387619e+02): complex,
                       (5.139626667737e+02, 5.103976610617e+02): complex,
                       (5.137423460082e+02, 5.105948019802e+02): complex,
                       (5.135547056878e+02, 5.107404165783e+02): complex,
                       (5.133910925466e+02, 5.108576573661e+02): complex,
                       (5.132470705390e+02, 5.109577278523e+02): complex,
                       (5.131197729984e+02, 5.110460304483e+02): complex,
                       (5.130070319283e+02, 5.111252433800e+02): complex,
                       (5.129070537032e+02, 5.111968077718e+02): complex,
                       (5.128182883502e+02, 5.112616233064e+02): complex,
                       (5.127393733383e+02, 5.113203605551e+02): complex,
                       (5.126691062020e+02, 5.113735928093e+02): complex,
                       (5.126064276004e+02, 5.114218460548e+02): complex,
                       (5.125504076570e+02, 5.114656139760e+02): complex,
                       (5.125002331720e+02, 5.115053595966e+02): complex,
                       (5.124551951846e+02, 5.115415130407e+02): complex,
                       (5.124146770029e+02, 5.115744692211e+02): complex];
      when NPB.C do return 
                      [(5.195078707457e+02, 5.149019699238e+02):complex,
                       (5.155422171134e+02, 5.127578201997e+02):complex,
                       (5.144678022222e+02, 5.122251847514e+02):complex,
                       (5.140150594328e+02, 5.121090289018e+02):complex,
                       (5.137550426810e+02, 5.121143685824e+02):complex,
                       (5.135811056728e+02, 5.121496764568e+02):complex,
                       (5.134569343165e+02, 5.121870921893e+02):complex,
                       (5.133651975661e+02, 5.122193250322e+02):complex,
                       (5.132955192805e+02, 5.122454735794e+02):complex,
                       (5.132410471738e+02, 5.122663649603e+02):complex,
                       (5.131971141679e+02, 5.122830879827e+02):complex,
                       (5.131605205716e+02, 5.122965869718e+02):complex,
                       (5.131290734194e+02, 5.123075927445e+02):complex,
                       (5.131012720314e+02, 5.123166486553e+02):complex,
                       (5.130760908195e+02, 5.123241541685e+02):complex,
                       (5.130528295923e+02, 5.123304037599e+02):complex,
                       (5.130310107773e+02, 5.123356167976e+02):complex,
                       (5.130103090133e+02, 5.123399592211e+02):complex,
                       (5.129905029333e+02, 5.123435588985e+02):complex,
                       (5.129714421109e+02, 5.123465164008e+02):complex];
      when NPB.D do return 
                      [(5.122230065252e+02, 5.118534037109e+02): complex,
                       (5.120463975765e+02, 5.117061181082e+02): complex,
                       (5.119865766760e+02, 5.117096364601e+02): complex,
                       (5.119518799488e+02, 5.117373863950e+02): complex,
                       (5.119269088223e+02, 5.117680347632e+02): complex,
                       (5.119082416858e+02, 5.117967875532e+02): complex,
                       (5.118943814638e+02, 5.118225281841e+02): complex,
                       (5.118842385057e+02, 5.118451629348e+02): complex,
                       (5.118769435632e+02, 5.118649119387e+02): complex,
                       (5.118718203448e+02, 5.118820803844e+02): complex,
                       (5.118683569061e+02, 5.118969781011e+02): complex,
                       (5.118661708593e+02, 5.119098918835e+02): complex,
                       (5.118649768950e+02, 5.119210777066e+02): complex,
                       (5.118645605626e+02, 5.119307604484e+02): complex,
                       (5.118647586618e+02, 5.119391362671e+02): complex,
                       (5.118654451572e+02, 5.119463757241e+02): complex,
                       (5.118665212451e+02, 5.119526269238e+02): complex,
                       (5.118679083821e+02, 5.119580184108e+02): complex,
                       (5.118695433664e+02, 5.119626617538e+02): complex,
                       (5.118713748264e+02, 5.119666538138e+02): complex,
                       (5.118733606701e+02, 5.119700787219e+02): complex,
                       (5.118754661974e+02, 5.119730095953e+02): complex,
                       (5.118776626738e+02, 5.119755100241e+02): complex,
                       (5.118799262314e+02, 5.119776353561e+02): complex,
                       (5.118822370068e+02, 5.119794338060e+02): complex];
      when NPB.E do return 
                      [(5.121601045346e+02, 5.117395998266e+02): complex,
                       (5.120905403678e+02, 5.118614716182e+02): complex,
                       (5.120623229306e+02, 5.119074203747e+02): complex,
                       (5.120438418997e+02, 5.119345900733e+02): complex,
                       (5.120311521872e+02, 5.119551325550e+02): complex,
                       (5.120226088809e+02, 5.119720179919e+02): complex,
                       (5.120169296534e+02, 5.119861371665e+02): complex,
                       (5.120131225172e+02, 5.119979364402e+02): complex,
                       (5.120104767108e+02, 5.120077674092e+02): complex,
                       (5.120085127969e+02, 5.120159443121e+02): complex,
                       (5.120069224127e+02, 5.120227453670e+02): complex,
                       (5.120055158164e+02, 5.120284096041e+02): complex,
                       (5.120041820159e+02, 5.120331373793e+02): complex,
                       (5.120028605402e+02, 5.120370938679e+02): complex,
                       (5.120015223011e+02, 5.120404138831e+02): complex,
                       (5.120001570022e+02, 5.120432068837e+02): complex,
                       (5.119987650555e+02, 5.120455615860e+02): complex,
                       (5.119973525091e+02, 5.120475499442e+02): complex,
                       (5.119959279472e+02, 5.120492304629e+02): complex,
                       (5.119945006558e+02, 5.120506508902e+02): complex,
                       (5.119930795911e+02, 5.120518503782e+02): complex,
                       (5.119916728462e+02, 5.120528612016e+02): complex,
                       (5.119902874185e+02, 5.120537101195e+02): complex,
                       (5.119889291565e+02, 5.120544194514e+02): complex,
                       (5.119876028049e+02, 5.120550079284e+02): complex];
      when NPB.F do return 
                      [(5.119892866928e+02, 5.121457822747e+02): complex,
                       (5.119560157487e+02, 5.121009044434e+02): complex,
                       (5.119437960123e+02, 5.120761074285e+02): complex,
                       (5.119395628845e+02, 5.120614320496e+02): complex,
                       (5.119390371879e+02, 5.120514085624e+02): complex,
                       (5.119405091840e+02, 5.120438117102e+02): complex,
                       (5.119430444528e+02, 5.120376348915e+02): complex,
                       (5.119460702242e+02, 5.120323831062e+02): complex,
                       (5.119492377036e+02, 5.120277980818e+02): complex,
                       (5.119523446268e+02, 5.120237368268e+02): complex,
                       (5.119552825361e+02, 5.120201137845e+02): complex,
                       (5.119580008777e+02, 5.120168723492e+02): complex,
                       (5.119604834177e+02, 5.120139707209e+02): complex,
                       (5.119627332821e+02, 5.120113749334e+02): complex,
                       (5.119647637538e+02, 5.120090554887e+02): complex,
                       (5.119665927740e+02, 5.120069857863e+02): complex,
                       (5.119682397643e+02, 5.120051414260e+02): complex,
                       (5.119697238718e+02, 5.120034999132e+02): complex,
                       (5.119710630664e+02, 5.120020405355e+02): complex,
                       (5.119722737384e+02, 5.120007442976e+02): complex,
                       (5.119733705802e+02, 5.119995938652e+02): complex,
                       (5.119743666226e+02, 5.119985735001e+02): complex,
                       (5.119752733481e+02, 5.119976689792e+02): complex,
                       (5.119761008382e+02, 5.119968675026e+02): complex,
                       (5.119768579280e+02, 5.119961575929e+02): complex];
      otherwise do halt("Unknown class");
    }
}



